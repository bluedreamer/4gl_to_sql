#ifndef TABLE
#define TABLE

#include <string>
#include <vector>
#include <map>
#include <optional>

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
      void addIndex( const Index &index );
      void addField( const Field &field );
      std::string getDescription() const;
      void reset();
      void setName( const std::string &name );
      void setDescription( const std::string &s );
      void setDumpName( const std::string &s );
      void setLabel( const std::string &s );
      void setValexp( const std::string &s );
      void setValmsg( const std::string &s );
      static std::string schema();
      std::string insertStatement() const;
      std::string fieldinsertStatement() const;
      template<class T>
      void printOptional( std::ostream &os, const T &t, bool comma = true ) const
      {
         if( t )
         {
            os << '\''  << t.value() << '\'';
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
      std::optional< std::string > area_;
      std::optional< std::string > description_;
      std::optional< std::string > dumpName_;
      std::optional< std::string > label_;
      std::optional< std::string > valexp_;
      std::optional< std::string > valmsg_;

      Fields fields_;
      Indexes indexes_;
      typedef std::map<std::string, Field> FieldMap;
      FieldMap extentFields_;
};

#endif
