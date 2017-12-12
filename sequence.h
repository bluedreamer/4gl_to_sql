#ifndef SEQUENCE_H
#define SEQUENCE_H

#include <optional>

#include "util.h"

class Sequence
{
   public:
      Sequence() { reset(); }
      void reset();
      void setName( const std::string &s ) { name_ = trimquote( s ); }
      void setInitial( int i ) { initial_ = i; }
      void setIncrement( int i ) { increment_ = i; }
      void setCylceOnLimit( bool b ) { cycleOnLimit_ = b; }
      void setMinVal( int i ) { minVal_ = i; }
      void setMaxVal( int i ) { maxVal_ = i; }
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
