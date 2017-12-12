#include <iostream>
#include <stdexcept>
#include <sstream>
#include <iomanip>

#include <boost/regex.hpp>
#include <boost/lexical_cast.hpp>

#include "field.h"
#include "util.h"
#include "database.h"

void Field::reset()
{
   name_.clear();
   tableName_.reset();
   type_.reset();
   order_.reset();
   mandatory_.reset();
   length_.reset();
   description_.reset();
   caseSensitive_.reset();
   decimals_.reset();
   position_.reset();
   initial_.reset();
   format_.reset();
   label_.reset();
   columnLabel_.reset();
   help_.reset();
   valexp_.reset();
   valmsg_.reset();
   extent_.reset();
   extentOrder_ = 0;
}

std::string Field::getMysqlType() const
{
   std::ostringstream strm;
   switch( type_.get() )
   {
      case character:
         strm << "VARCHAR(" << length_.get() << ")";
         break;
      case integer:
         strm << "INT";
         break;
      case date:
         strm << "DATE";
         break;
      case logical:
         strm << "TINYINT(1)";
         break;
      case decimal:
         strm << "DECIMAL(" << length_.get() << ", " << decimals_.get() << ")";
         break;
      case datetimetz:
         strm << "DATETIME";
         break;
      case recid:
         // Ignore recid for moment - just auto_inc field that usually doesn't get dumped out
         // so its not in the .d file anyway
         break;
      case raw:
         strm << "BLOB";
         break;
      default:
         strm << "No mapping for field type: [" << type_.get() << "]\n";
         throw std::logic_error( strm.str() );
   }
   return strm.str();
}

std::string Field::getForeignKeyDef() const
{
   std::string rc;
   return rc;
}

std::string Field::getMysqlNull() const
{
   if( mandatory_ )
   {
      return " NOT NULL ";
   }
   return " NULL ";
}

std::string Field::getMysqlComment() const
{
   if( !description_ )
   {
      return std::string();
   }
   std::ostringstream strm;

   strm << " COMMENT '" << description_.get() << "' ";
   return strm.str();
}

std::ostream &operator<<( std::ostream &os, const Field &field)
{
   // not outputing recid's
   if( field.type_.get() != Field::recid)
   {
      os << "   `" << field.name_ << "` " << field.getMysqlType() << field.getMysqlNull() << field.getMysqlComment();
   }
   return os;
}

void Field::readData( std::istream &is )
{
   std::ostringstream strm;
   const datatype type = type_.get();
   switch( type )
   {
      case integer:
      {
         std::string data;
         is >> data;
         if( data.length() == 1 && data[0] == '?' || data == "\"?\"" )
         {
            //stupid nulls
            strm << "NULL";
            // skip space past ?
            is.get();
         }
         else if( data == "\"\"" )
         {
            strm << "NULL";
            // skip space past ""
         }
         else
         {
            strm << boost::lexical_cast<int>( data );
         }
      }
      break;

      case date:
      {
         std::string data;
         is >> data;
         boost::regex date_exp( "([0-9][0-9])/([0-9][0-9])/([0-9]+)" );
         boost::smatch parts;
         if( data == "\"\"" )
         {
            strm << "'0000-00-00'";
         }
         else if( boost::regex_match( data, parts, date_exp ) )
         {
            int day = boost::lexical_cast<int>( parts[2].str() );
            int month = boost::lexical_cast<int>( parts[1].str() );
            int year = boost::lexical_cast<int>( parts[3].str() );
            if( year >=1000 )
            {
               // do nothing
            }
            else if( year >= 50 )
            {
               year += 1900; 
            }
            else
            {
               year += 2000;
            }
            strm.fill('0');
            strm << '\'' << std::setw( 4 ) << year
                 << '-' << std::setw( 2 ) << month
                 << '-' << std::setw( 2 ) << day
                 << '\'';
         }
         else if( data.length() == 1 && data[0] == '?' )
         {
            // some sort of null I think
            strm << "NULL";
            // skip space past ?
            is.get();
         }
         else
         {
            strm << boost::lexical_cast<int>( data );
         }
      }
      break;

      case character:
      {
         size_t char_count = 0;
         char c = is.get(); ++char_count;
         if( c == ' ' )
         {
            c = is.get(); ++char_count;
         }
         if( c != '"' )
         {
            if( c == '?' )
            {
               strm << "NULL";
               break;
            }
            throw std::logic_error( "Bad string field for field: " + name_ );
         }
         // throw away starting quote
         c = is.get(); ++char_count;
         // lets check not escaped quote at start
         while( c == '"' && is.peek() == '"' )
         {
            if( char_count > length_.get() + 10) throw std::logic_error( "too longA " + strm.str());
            strm << '"';
            // skip the peek
            c = is.get(); ++char_count;
            // get next real char
            c = is.get(); ++char_count;
         }
         while( c != '"' )
         {
            if( char_count > length_.get() + 10) break;
            if( c == '\\' )
            {
               strm << '\\';
            }
            if( c == '\'' )
            {
               strm << '\\';
            }
            strm << c;
            c = is.get(); ++char_count;
            // check if this is an escaped quote
            while( c == '"' && is.peek() == '"' )
            {
            if( char_count > length_.get() + 10) throw std::logic_error( "too longC " + strm.str());
               strm << '"';
               // skip the peek
               c = is.get(); ++char_count;
               // get next real char
               c = is.get(); ++char_count;
            }
         }
         std::string temp = strm.str();
         trim ( temp );
         strm.str("");
         if( temp.length() == 0 )
         {
            if( mandatory_ )
            {
               strm << "''";
            }
            else
            {
               strm << "NULL";
            }
            // skip space past ?
            is.get();
         }
         else
         {
            strm << '\'' << temp << '\'';
         }
      }
      break;

      case logical:
      {
         std::string data;
         is >> data;
         if( data == "no" )
         {
            strm << "0";
         }
         else if ( data == "yes" )
         {
            strm << "1";
         }
         else if ( data == "?" )
         {
            strm << "NULL";
         }
         else
         {
            throw std::logic_error(" Unknown logical setting for field: " + name_ + " [" + data + "]");
         }
      }
      break;
      case decimal:
      {
         std::string data;
         is >> data;
         if( data.length() == 1 && data[0] == '?' )
         {
            //stupid nulls
            strm << "NULL";
         }
         else
         {
            strm << boost::lexical_cast<double>( data );
         }
      }
      break;
      case recid:
         // Record id is not usuall in dump file
         break;

      default:
      {
         std::ostringstream error;
         error << __PRETTY_FUNCTION__ << " havent written this field type yet " << type;
         throw std::logic_error(error.str());
      }
   }
   fieldData_ = strm.str();
}

std::string Field::schemaName_ = "INFO_fields";

std::string Field::schema()
{
   std::ostringstream strm;
   strm << "DROP TABLE IF EXISTS " << schemaName_ << ";\n";
   strm << "CREATE TABLE " << schemaName_ << "\n(\n";
   strm << "name VARCHAR(50) NOT NULL,\n";
   strm << "tablename VARCHAR(50) NOT NULL,\n";
   strm << "PRIMARY KEY (name, tablename),\n";
   strm << "type ENUM('character', 'integer', 'date', 'logical', 'decimal'),\n";
   strm << "`order` INT,\n";
   strm << "mandatory TINYINT(1),\n";
   strm << "length INT,\n";
   strm << "description VARCHAR(255),\n";
   strm << "casesensitive TINYINT(1),\n";
   strm << "decimals INT,\n";
   strm << "position INT,\n";
   strm << "initial VARCHAR(50),\n";
   strm << "format VARCHAR(50),\n";
   strm << "label VARCHAR(50),\n";
   strm << "columnlabel VARCHAR(80),\n";
   strm << "`help` VARCHAR(200),\n";
   strm << "valexp VARCHAR(255),\n";
   strm << "valmsg VARCHAR(200),\n";
   strm << "extent INT\n";
   strm << ") ENGINE=InnoDB COMMENT = 'Add field records from Dump File';\n";
   return strm.str();
}

std::string Field::insertStatement() const
{
   std::ostringstream strm;
   strm << "INSERT INTO `" << schemaName_ << "`";
   //<< " (name, tablename, type, `order`, mandatory, length, description, casesensitive, decimals, position, initial, format, label, columnlabel, help, valexp, valmsg, extent) ";
   strm << " VALUES(";
   strm << '\'' << name_ << "',";
   strm << '\'' << tableName_.get() << "',";
   strm << type_.get() + 1 << ", ";
//   printOptional( strm, type_, false );
   printOptional( strm, order_, false );
   printOptional( strm, mandatory_, false );
   printOptional( strm, length_, false );
   printOptional( strm, description_ );
   printOptional( strm, caseSensitive_, false );
   printOptional( strm, decimals_, false );
   printOptional( strm, position_, false );
   printOptional( strm, initial_ );
   printOptional( strm, format_ );
   printOptional( strm, label_ );
   printOptional( strm, columnLabel_ );
   printOptional( strm, help_ );
   printOptional( strm, valexp_ );
   printOptional( strm, valmsg_ );
   printOptional( strm, extent_, false, false );
   strm << ");\n";
   return strm.str();
}
