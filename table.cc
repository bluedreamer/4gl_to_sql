#include <sstream>
#include <fstream>
#include <algorithm>
#include <iostream>
#include <stdexcept>
#include <unistd.h>
#include "table.h"
#include "database.h"
#include "util.h"
#include "config.h"

void Table::reset()
{
   tableName_.clear();
   area_.reset();
   description_.reset();
   dumpName_.reset();
   label_.reset();
   valexp_.reset();
   valmsg_.reset();
   fields_.clear();
   indexes_.clear();
   extentFields_.clear();
}

bool compField(Field l, Field r)
{
   if( l.getOrder() == r.getOrder() )
   {
      return l.getExtentOrder() < r.getExtentOrder();
   }
   return l.getOrder() < r.getOrder();
}


void Table::sortFields()
{
   std::sort( fields_.begin(), fields_.end(), compField );
}

std::string Table::getForeignKeysDefs() const
{
   std::ostringstream strm;
   for( size_t i=0; i< fields_.size(); ++i)
   {
      const std::string rc = fields_[i].getForeignKeyDef();
      if( !rc.empty() )
      {
         strm << rc;
         if( i + 1 != fields_.size() )
         {
            strm << ",\n";
         }
      }
   }
   return strm.str();
}

std::ostream &operator<<(std::ostream &os, const Table &table)
{
   os << "DROP TABLE IF EXISTS `"<< table.tableName_ << "`;\n";
   os << "CREATE TABLE `" << table.tableName_ << "`\n";
   os << "(\n";
   for( Table::Fields::const_iterator i=table.fields_.begin(),end=table.fields_.end(); i!=end; ++i)
   {
      os << (*i);
      if( i + 1 != end )
      {
         os << ",\n";
      }
      else
      {
         os << "\n";
      }
   }
   if( table.indexes_.size() && outputTableIndexes )
   {
      os << ", ";
      for( Table::Indexes::const_iterator i=table.indexes_.begin(),end=table.indexes_.end(); i!=end; ++i)
      {
         std::vector<std::string> fieldsInIndex;
         i->getFieldList( fieldsInIndex );

         if( table.areAnyFieldsInExtentList( fieldsInIndex ) )
         {
            os << "#" << (*i) << "\n";
            os << "# Ignoring extent field as index\n";
         }
         else
         {
            os << (*i);
         }
         if( i + 1 != end )
         {
            os << ",\n";
         }
         else
         {
            os << "\n";
         }
      }
   }
   os << ") ";
   os << " ENGINE=InnoDB COMMENT = '" << table.getDescription() << "' ";
   os << ";\n";
   return os;
}

std::string Table::getDumpName() const
{
   std::ostringstream strm;
   if( dumpName_ )
   {
      strm << dumpName_.get();
   }
   else
   {
      strm << tableName_;
   }
   return strm.str();
}

void Table::convertDumpToSql()
{
   std::string infile( getDumpName() + ".d" );
   std::string outfile( getDumpName() + ".sql" );
   size_t lineno=0;
   try 
   {
      std::ifstream in( infile.c_str() );
      if( !in )
      {
         return;
      }
      std::ofstream out( outfile.c_str() );

      out << "LOCK TABLES `" << tableName_ << "` WRITE;\n";
      out << "ALTER TABLE `" << tableName_ << "` DISABLE KEYS;\n";
      out << "BEGIN;\n";
      out << "DELETE FROM `" << tableName_ << "`;\n";
      std::string line;
      while( getline( in, line ) )
      {
         ++lineno;
         if( line == "." )
         {
            break;
         }
         std::istringstream instrm( line );
   //      std::cout << tableName_ << ':' << line << std::endl;
         out << "INSERT INTO `" << tableName_ << "` VALUES(";
         for( Table::Fields::iterator i=fields_.begin(),end=fields_.end(); i!=end; ++i)
         {
            i->readData( instrm );
            i->printData( out );
            if( i + 1 != end )
            {
               out << ", ";
            }
         }
         out << ");\n";

      }
      out << "COMMIT;\n";
      out << "ALTER TABLE `" << tableName_ << "` ENABLE KEYS;\n";
      out << "UNLOCK TABLES;\n";
   }
   catch( const std::exception &e )
   {
      std::cerr << "Bollocks: table:" << tableName_ << " file:" << infile << " line: " << lineno << ' ' << e.what() << "\n";
      throw;
   }
}

std::string Table::schemaName_ = "INFO_tables";

std::string Table::schema()
{
   std::ostringstream strm;
   strm << "DROP TABLE IF EXISTS " << schemaName_ << ";\n";
   strm << "CREATE TABLE " << schemaName_ << "\n(\n";
   strm << "name VARCHAR(100),\n";
   strm << "PRIMARY KEY (name),\n";
   strm << "area VARCHAR(50),\n";
   strm << "description VARCHAR(255),\n";
   strm << "dumpname VARCHAR(50),\n";
   strm << "label VARCHAR(100),\n";
   strm << "valexp VARCHAR(255),\n";
   strm << "valmsg VARCHAR(255),\n";
   strm << "no_fields INT,\n";
   strm << "no_indexes INT\n";
   strm << ") ENGINE=InnoDB COMMENT = 'Add table records from Dump File';\n";
   return strm.str();
}

std::string Table::insertStatement() const
{
   std::ostringstream strm;
   strm << "INSERT INTO `" << schemaName_ << "` (name, area, description, dumpname, label, valexp, valmsg, no_fields, no_indexes) ";
   strm << "VALUES(";
   strm << '\'' << tableName_ << "',";
   printOptional( strm, area_ );
   printOptional( strm, description_ );
   printOptional( strm, dumpName_ );
   printOptional( strm, label_ );
   printOptional( strm, valexp_ );
   printOptional( strm, valmsg_ );
   strm << fields_.size() << ',';
   strm << indexes_.size();
   strm << ");\n";
   return strm.str();
}

std::string Table::fieldinsertStatement() const
{
   std::ostringstream strm;
   std::string debug;
   strm << "DELETE FROM `" << tableName_ << "`;\n";
   for( Table::Fields::const_iterator i=fields_.begin(),end=fields_.end(); i!=end; ++i)
   {
      strm << i->insertStatement(); 
      debug=strm.str();
   }
   return strm.str();
}

std::string Table::getDescription() const
{
   if( description_ )
   {
      return description_.get();
   }
   return std::string();
}

void Table::addField( const Field &field )
{ 
   // If the field has EXTENT set then we are going to explode 
   // it here
   const size_t extent = field.getExtent();
   if( extent == 1 )
   {
      fields_.push_back( field ); 
   }
   else
   {
      // save original in field map
      extentFields_.insert( std::make_pair( field.getName(), field ) );
      for(size_t i = 1; i <= extent; ++i)
      {
         std::ostringstream newname;
         // expects quoted field name
         newname << '"' << field.getName() << "_extent" << i << '"';
         Field newField( field );
         newField.setName( newname.str() );
         newField.setLength( field.getLength() / extent );
         newField.setExtentOrder( i );
         fields_.push_back( newField );
      }
   }
}

bool Table::areAnyFieldsInExtentList( const std::vector<std::string> &list ) const
{
   for( size_t i = 0; i < list.size(); ++i)
   {
      FieldMap::const_iterator it = extentFields_.find( list[ i ] );
      if( it != extentFields_.end() )
      {
         return true;
      }
   }
   return false;
}

std::string Table::dumpIndexesAsAlterTable() const
{
   std::ostringstream strm;
   for( Indexes::const_iterator i = indexes_.begin(),end=indexes_.end(); i!=end; ++i)   
   {
      strm << i->dumpIndexesAsAlterTable( tableName_ ) << std::endl;
   }
   return strm.str();
}
