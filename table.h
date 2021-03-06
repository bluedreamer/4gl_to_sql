#ifndef TABLE
#define TABLE

#include <string>
#include <vector>
#include <map>

#include <boost/optional.hpp>

#include "util.h"

#include "field.h"
#include "index.h"

class Table
{
   public:
      Table() { reset(); }
      friend std::ostream &operator<<(std::ostream &os, const Table &table);

      typedef std::vector<Field> Fields;
      typedef std::vector<Index> Indexes;
      void sortFields();
      void convertDumpToSql();
      std::string getForeignKeysDefs() const;
      void addIndex( const Index &index ) { indexes_.push_back( index ); }
      void addField( const Field &field );
      std::string getDescription() const;
      void reset();
      void setName( const std::string &name ) { tableName_ = trimquote( name ); }
      void setDescription( const std::string &s ) { description_ = trimquote( s ); escapestring( description_.get() ); }
      void setDumpName( const std::string &s ) { dumpName_ = trimquote( s ); }
      void setLabel( const std::string &s ) { label_ = trimquote( s ); }
      void setValexp( const std::string &s ) { valexp_ = trimquote( s ); }
      void setValmsg( const std::string &s ) { valmsg_ = trimquote( s ); escapestring( valmsg_.get() ); }
      static std::string schema();
      std::string insertStatement() const;
      std::string fieldinsertStatement() const;
      template<class T>
      void printOptional( std::ostream &os, const T &t, bool comma = true ) const
      {
         if( t )
         {
            os << '\''  << t.get() << '\'';
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
      std::string dumpIndexesAsAlterTable() const;
   private:
      std::string getDumpName() const;
      bool areAnyFieldsInExtentList( const std::vector<std::string> &list ) const;
      static std::string schemaName_;
      std::string tableName_;
      boost::optional< std::string > area_;
      boost::optional< std::string > description_;
      boost::optional< std::string > dumpName_;
      boost::optional< std::string > label_;
      boost::optional< std::string > valexp_;
      boost::optional< std::string > valmsg_;

      Fields fields_;
      Indexes indexes_;
      typedef std::map<std::string, Field> FieldMap;
      FieldMap extentFields_;
};

#endif
