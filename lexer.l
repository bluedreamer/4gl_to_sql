%{
#include <iostream>
#include <string>
#define YY_DECL extern "C" int yylex()
enum datatype {character, integer, date, logical, decimal, datetimetz, recid, raw};
int line_num=1;
#include "parser.tab.h"
%}
%%
[ \t]          ;
"ADD SEQUENCE" { return ADDSEQUENCE; }
"ADD TABLE"    { return ADDTABLE; }
"ADD FIELD"    { return ADDFIELD; }
"ADD INDEX"    { return ADDINDEX; }
"AREA"         { return AREA; }
"WORD"         { return WORD; }
"DESCRIPTION"  { return DESCRIPTION; }
"DUMP-NAME"    { return DUMPNAME; }
"INCREMENT"    { return INCREMENT; }
"CYCLE-ON-LIMIT" { return CYCLEONLIMIT; }
"MIN-VAL"      { return MINVAL; }
"MAX-VAL"      { return MAXVAL; }
"ASCENDING"    { return ASCENDING; }
"DECIMALS"    { return DECIMALS; }
"DESCENDING"   { return DESCENDING; }
"EXTENT"       { return EXTENT; }
"AS"           { return AS; }
"ON"           { return ON; }
"OF"           { return OF; }
\"[^"]*\"      { 
                 char *ptr=yytext;
                 while(*ptr)
                 {
                     if(*ptr=='\n')
                     {
                        *ptr=' ';
                        ++line_num; 
                     }
                     ++ptr;
                 }
                  yylval.sval = strdup( yytext); return QSTRING; }
-?[0-9]+         { yylval.ival = atoi( yytext ); return INT; }
"yes"|"no"     { yylval.bval = strcmp(yytext, "yes")==0?1:0; return LOGICAL; }
"FORMAT"       { return FORMAT; }
"INITIAL"      { return INITIALVAL; }
"LABEL"        { return LABEL; }
"POSITION"     { return POSITION; }
"MAX-WIDTH"    { return MAXWIDTH; }
"COLUMN-LABEL" { return COLUMNLABEL; }
"ORDER"        { return ORDER; }
"UNIQUE"       { return UNIQUE; }
"PRIMARY"      { return PRIMARY; }
"INDEX-FIELD"  { return INDEXFIELD; }
"HELP"         { return HELP; }
"CASE-SENSITIVE" { return CASESENSITIVE; }
"VALMSG"       { return VALMSG; }
"VALEXP"       { return VALEXP; }
"CATEGORY"     { return CATEGORY; }
"FROZEN"       { return FROZEN; }
"HIDDEN"       { return HIDDEN; }
"MANDATORY"    { return MANDATORY; }
"ABBREVIATED"  { return ABBREVIATED; }
"UPDATE DATABASE"  { return UPDATEDATABASE; }
"?"            { yylval.sval = strdup( "\"NULL\"" ); return DBNULL; }
"character"|"integer"|"date"|"logical"|"decimal"|"datetime-tz"|"recid"|"raw" { std::string str(yytext);
      if( str == "character")
         yylval.ival = character;
      else if( str == "integer" )
         yylval.ival = integer;
      else if ( str == "date")
         yylval.ival=date;
      else if( str== "logical")
         yylval.ival=logical;
      else if( str == "decimal")
         yylval.ival=decimal;
      else if( str == "datetime-tz" )
          yylval.ival=datetimetz;
      else if( str == "recid" )
          yylval.ival=recid;
      else if( str == "raw" )
          yylval.ival=raw;
      
      return DATATYPE;
   }
^\.$           { return EOFDEFFILE; }
\n             { ++line_num; }
%%
