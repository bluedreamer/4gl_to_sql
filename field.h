#ifndef FIELD_H
#define FIELD_H

#include <istream>

#include <boost/optional.hpp>

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
      int getOrder() const { if( order_ ) return order_.get(); else return -1; }
      size_t getExtentOrder() const { return extentOrder_; }
      size_t getExtent() const { if ( extent_ ) return extent_.get(); else return 1; }
      const std::string &getName() const { return name_; }
      int getLength() const { return length_.get(); }
      void readData( std::istream &is );
      void printData( std::ostream &os ) { os << fieldData_; };
      std::string getForeignKeyDef() const;
      void reset();
      void setDescription( const std::string &s ) { description_ = trimquote( s ); escapestring( description_.get() ); }
      void setCaseSensitive() { caseSensitive_ = true; }
      void setDecimal( int d ) { decimals_ = d; }
      void setOrder( int d ) { order_ = d; }
      void setMandatory() { mandatory_ = true; }
      void setPosition( int d ) { position_ = d; }
      void setName( const std::string &name ) { name_ = trimquote( name ); }
      void setType( int type ) { type_ = (datatype)type; }
      void setLength( int len ) { length_ = len; }
      void setTable( const std::string &s ) { tableName_ = trimquote( s ); }
      void setInitial( const std::string &s ) { initial_ = trimquote( s ); }
      void setFormat( const std::string &s ) { format_ = trimquote( s ); }
      void setLabel( const std::string &s ) { label_ = trimquote( s ); escapestring( label_.get() ); }
      void setColumnLabel( const std::string &s ) { columnLabel_ = trimquote( s ); escapestring( columnLabel_.get() ); }
      void setHelp( const std::string &s ) { help_ = trimquote( s ); escapestring( help_.get() );  }
      void setValexp( const std::string &s ) { valexp_ = trimquote( s ); escapestring( valexp_.get() ); }
      void setValmsg( const std::string &s ) { valmsg_ = trimquote( s ); escapestring( valmsg_.get() ); }
      void setExtent( int i ) { extent_ = i; }
      void setExtentOrder( size_t i ) { extentOrder_ = i; }
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
            os << t.get();
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
      boost::optional<std::string> tableName_;
      boost::optional<datatype> type_;
      boost::optional<int> order_;
      boost::optional<int> mandatory_;
      boost::optional<int> length_;
      boost::optional<std::string> description_;
      boost::optional<int> caseSensitive_;
      boost::optional<int> decimals_;
      boost::optional<int> position_;
      boost::optional<std::string> initial_;
      boost::optional<std::string> format_;
      boost::optional<std::string> label_;
      boost::optional<std::string> columnLabel_;
      boost::optional<std::string> help_;
      boost::optional<std::string> valexp_;
      boost::optional<std::string> valmsg_;
      boost::optional<int> extent_;
      std::string fieldData_;
      // we create this when exploing extent fields
      size_t extentOrder_;
};

#endif
