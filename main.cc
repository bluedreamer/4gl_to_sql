#include <iostream>
#include <fstream>
#include <cstdio>
#include <cstdlib>

#include "database.h"
#include "config.h"


extern "C" int yylex();
int yyparse();
extern "C" FILE *yyin;

void yyerror(const char *s);

extern int line_num;
Database db;

// poor mans config
bool outputTableIndexes = true;

int main(int argc, char *argv[])
{
   const char *fname="default.df";
   if( argc > 1 )
   {
      fname = argv[1];
   }

   yyin = fopen( fname, "r" );
   if( !yyin )
   {
      perror("fopen");
      exit(1);
   }
   do
   {
      yyparse();
   } while( !feof( yyin ) );
   fclose( yyin );

   try
   {
      std::cout << db;
//      exit(1);
      db.convertDumpToSql();
//      std::cout << db.dumpIndexesAsAlterTable();
   }
   catch( const std::exception &e )
   {
      std::cerr << "EXCEPTION: "  << e.what() << std::endl;
   }


   return 0;
}

void yyerror(const char *s)
{
   std::cerr << "PARSING ERROR: " << line_num << " " << s << std::endl;
   exit(1);
}

