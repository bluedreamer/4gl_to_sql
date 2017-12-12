#ifndef FIELD_H
#define FIELD_H

#include <istream>
#include <optional>

#include "util.h"

class Field
{
   public:
      enum datatype {character, integer, date, logical, decimal, datetimetz, recid, raw};
      Field() { reset(); }
      friend std::ostream &operator<<( std::ostream &os, const Field &field);
      std::string getMysqlType() const;
      std::string getMysqlNull() const;
      std::string getMysqlComment() const;
      int getOrder() const;
      size_t getExtentOrder() const;
      size_t getExtent() const;
      const std::string &getName() const;
      int getLength() const;
      void readData( std::istream &is );
      void printData( std::ostream &os );;
      std::string getForeignKeyDef() const;
      void reset();
      void setDescription( const std::string &s );
      void setCaseSensitive();
      void setDecimal( int d );
      void setOrder( int d );
      void setMandatory();
      void setPosition( int d );
      void setName( const std::string &name );
      void setType( int type );
      void setLength( int len );
      void setTable( const std::string &s );
      void setInitial( const std::string &s );
      void setFormat( const std::string &s );
      void setLabel( const std::string &s );
      void setColumnLabel( const std::string &s );
      void setHelp( const std::string &s );
      void setValexp( const std::string &s );
      void setValmsg( const std::string &s );
      void setExtent( int i ) { extent_ = i; }
      void setExtentOrder( size_t i );
      static std::string schema();
      std::string insertStatement() const;
      template<class T>
      void printOptional( std::ostream &os, const T &t, bool quoted = true, bool comma = true ) const
      {
         if( t )
         {
            if( quoted )
            {
               os << '\'';
            }
            os << t.value();
            if( quoted )
            {
               os << '\'';
            }
         }
         else
         {
            os << "NULL";
         }

         if( comma )
         {
            os << ',';
         }
      }
   private:
      static std::string schemaName_;
      std::string name_;
      std::optional<std::string> tableName_;
      std::optional<datatype> type_;
      std::optional<int> order_;
      std::optional<int> mandatory_;
      std::optional<int> length_;
      std::optional<std::string> description_;
      std::optional<int> caseSensitive_;
      std::optional<int> decimals_;
      std::optional<int> position_;
      std::optional<std::string> initial_;
      std::optional<std::string> format_;
      std::optional<std::string> label_;
      std::optional<std::string> columnLabel_;
      std::optional<std::string> help_;
      std::optional<std::string> valexp_;
      std::optional<std::string> valmsg_;
      std::optional<int> extent_;
      std::string fieldData_;
      // we create this when exploing extent fields
      size_t extentOrder_;
};

#endif
