#ifndef INDEX_H
#define INDEX_H

#include <istream>
#include <string>
#include <vector>

#include "util.h"

class Index
{
   public:
      Index() { reset(); }
      friend std::ostream &operator<<( std::ostream &os, const Index &index );
      void reset();
      void setUnique();
      void setPrimary() { primary_ = true; }
      void setName( const std::string &name );
      void setTable( const std::string &name );
      void setArea( const std::string &s );
      void setWord() { word_ = 1; }
      void addIndexField( const std::string &fieldName, bool direction);
      static std::string schema();
      std::string insertStatement() const;
      template<class T>
      void printOptional( std::ostream &os, const T &t, bool comma=true ) const
      {
         if( t )
         {
            os << t.value();
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
      const std::string &getName() const { return name_; }
      void getFieldList( std::vector<std::string> &list ) const;
      std::string dumpIndexesAsAlterTable( const std::string &tablename ) const;
   private:
      static std::string schemaName_;
      std::string name_;
      std::string tableName_;
      std::string area_;
      bool unique_;
      bool primary_;
      bool word_;
      std::vector<std::pair<std::string, bool> > fieldName_;
};

#endif
