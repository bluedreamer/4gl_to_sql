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
      void reset()
      {
         name_.clear();
         unique_ = false;
         primary_ = false;
         fieldName_.clear();
      }
      void setUnique() { unique_ = true; }
      void setPrimary() { primary_ = true; }
      void setName( const std::string &name ) { name_ = trimquote( name ); replacehypens( name_ ); }
      void setTable( const std::string &name ) { tableName_ = trimquote( name ); }
      void setArea( const std::string &s ) { area_  = trimquote( s ); }
      void setWord() { word_ = 1; }
      void addIndexField( const std::string &fieldName, bool direction)
      {
         fieldName_.push_back( std::make_pair( trimquote( fieldName ), direction ) );
      }
      static std::string schema();
      std::string insertStatement() const;
      template<class T>
      void printOptional( std::ostream &os, const T &t, bool comma=true ) const
      {
         if( t )
         {
            os << t.get();
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
