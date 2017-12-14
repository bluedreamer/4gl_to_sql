#include <iostream>
#include <stdexcept>
#include <sstream>
#include "index.h"
#include "util.h"
#include "database.h"

std::ostream &operator<<( std::ostream &os, const Index &index )
{
   if( index.primary_ )
   {
      // Seems like progress can have primary non unique keys
      // guess primary is alt for 1st key/main key
      if( index.unique_ )
      {
         os << "   PRIMARY KEY (";
      }
      else
      {
         os << "   INDEX `k_PRIMARY_" << index.name_ << "` (";
      }
   }
   else if( index.unique_ )
   {
      os << "   UNIQUE `k_" << index.name_ << "` (";
   }
   else
   {
      os << "   INDEX `k_" << index.name_ << "` (";
   }
   for(size_t i=0; i < index.fieldName_.size(); ++i)
   {
      os << "`" << index.fieldName_[i].first << "`";
      if( index.fieldName_[i].second )
      {
         os << " ASC ";
      }
      else
      {
         os << " DESC ";
      }
      if( i+1 != index.fieldName_.size() )
      {
         os << ", ";
      }
   }
   //os << ") COMMENT '" << index.name_ << "' ";
   os << ") ";
   return os;
}

std::string Index::schemaName_ = "INFO_indexes";

std::string Index::schema()
{
   std::ostringstream strm;
   strm << "DROP TABLE IF EXISTS " << schemaName_ << ";\n";
   strm << "CREATE TABLE " << schemaName_ << "\n(\n";
   strm << "name VARCHAR(50),\n";
   strm << "tablename VARCHAR(50),\n";
   strm << "area VARCHAR(50),\n";
   strm << "`unique` TINYINT(1),\n";
   strm << "`primary` TINYINT(1),\n";
   strm << "word TINYINT(1),\n";
   strm << "no_fields INT\n";
   strm << ") ENGINE=InnoDB COMMENT = 'Add indexes records from Dump File';\n";
   return strm.str();
}

std::string Index::insertStatement() const
{
}

void Index::getFieldList( std::vector<std::string> &list ) const
{
   for(size_t i=0; i < fieldName_.size(); ++i)
   {
      list.push_back( fieldName_[ i ].first );
   }
}

std::string Index::dumpIndexesAsAlterTable( const std::string &tablename ) const
{
   std::ostringstream strm;
   strm << "ALTER TABLE `" << tablename << "` ADD ";
   if( primary_ )
   {
      // Seems like progress can have primary non unique keys
      // guess primary is alt for 1st key/main key
      if( unique_ )
      {
         strm << "PRIMARY KEY (";
      }
      else
      {
         strm << "INDEX `k_PRIMARY_" << name_ << "` (";
      }
   }
   else if( unique_ )
   {
      strm << "UNIQUE `k_" << name_ << "` (";
   }
   else
   {
      strm << "INDEX `k_" << name_ << "` (";
   }
   for(size_t i=0; i < fieldName_.size(); ++i)
   {
      strm << "`" << fieldName_[i].first << "`";
      if( fieldName_[i].second )
      {
         strm << " ASC ";
      }
      else
      {
         strm << " DESC ";
      }
      if( i+1 != fieldName_.size() )
      {
         strm << ", ";
      }
   }
   //os << ") COMMENT '" << index.name_ << "' ";
   strm << ");";
   return strm.str();
}

void Index::reset()
{
   name_.clear();
   unique_ = false;
   primary_ = false;
   fieldName_.clear();
}

void Index::setName(const std::string &name)
{
   name_ = trimquote(name);
   replacehypens(name_);
}

void Index::setUnique()
{
   unique_ = true;
}

void Index::setTable(const std::string &name)
{
   tableName_ = trimquote(name);
}

void Index::addIndexField(const std::string &fieldName, bool direction)
{
   fieldName_.push_back(std::make_pair(trimquote(fieldName), direction));
}

void Index::setArea(const std::string &s)
{
   area_ = trimquote(s);
}
