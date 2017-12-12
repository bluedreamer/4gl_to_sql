#include <stdexcept>
#include <iostream>
#include "database.h"
#include "util.h"

void trim( std::string &str )
{
   str.erase( 0, str.find_first_not_of(' ') );
   str.erase( str.find_last_not_of(' ') + 1, std::string::npos );
}

std::string trimquote( const std::string &s )
{
   std::string str( s );
   trim( str );
   if( str.length() < 2 )
   {
      throw std::logic_error( "String too short for quote trim" );
   }

   if( str[0] != '"' || str[ str.length() -1 ] != '"' )
   {
      throw std::logic_error( "Not a quoted string: " + s );
   }
   str.erase( 0, 1 );
   str.erase( str.length() - 1 );
   // sometimes space padded inside quotes
   trim( str );
   return str;
}

void escapestring( std::string &str )
{
   std::string::size_type pos = 0;
   while( (pos = str.find("'", pos) ) != std::string::npos )
   {
      str.insert( pos, "\\" );
      pos += 2;
   }
}

void tolower( std::string &str )
{
   for(size_t i=0; i < str.length(); ++i)
   {
      str[i] = tolower( str[i] );
   }
}

void replacehypens( std::string &str )
{
   for(size_t i=0; i < str.length(); ++i)
   {
      if( str[i] == '-' )
      {
         str[i] = '_';
      }
   }
}
