#ifndef SEQUENCE_H
#define SEQUENCE_H

#include <optional>

#include "util.h"

class Sequence
{
   public:
      Sequence() { reset(); }
      void reset();
      void setName( const std::string &s );
      void setInitial( int i );
      void setIncrement( int i );
      void setCylceOnLimit( bool b );
      void setMinVal( int i );
      void setMaxVal( int i );
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
   private:
      static std::string schemaName_;
      std::string name_;
      std::optional<int> initial_;
      std::optional<int> increment_;
      std::optional<int> cycleOnLimit_;
      std::optional<int> minVal_;
      std::optional<int> maxVal_;
};

#endif
