%{
#include <cstdio>
#include <iostream>

#include "database.h"

extern "C" int yylex();
extern "C" int yyparser();
extern "C" FILE *yyin;

void yyerror(const char *s);

extern int line_num;

enum datatype {character, integer, date, logical, decimal};
Table table;
Field field;
Index index;
Sequence sequence;

bool index_direction=false;
%}

%union {
   int ival;
   char *sval;
   bool bval;
}

%token ADDTABLE AREA DESCRIPTION DUMPNAME UPDATEDATABASE
%token ADDFIELD ON AS FORMAT INITIALVAL LABEL POSITION MAXWIDTH COLUMNLABEL ORDER MANDATORY
%token VALMSG HELP OF ABBREVIATED WORD
%token ADDINDEX PRIMARY UNIQUE INDEXFIELD DESCENDING ASCENDING
%token ADDSEQUENCE INCREMENT CYCLEONLIMIT MINVAL MAXVAL EXTENT DECIMALS VALEXP CASESENSITIVE EOFDEFFILE
%token HIDDEN FROZEN CATEGORY

%token <ival> INT
%token <sval> QSTRING
%token <sval> DBNULL
%token <ival> DATATYPE
%token <bval> LOGICAL

%type <sval> nested_qstring
%type <sval> strornull

%%
dumpfile:
   optional_dbupdate sequences tables end 
   ;

optional_dbupdate: UPDATEDATABASE QSTRING
                   |
                   ;

end: EOFDEFFILE { return 0; }
     ;

sequences: sequences sequence
           | sequence
           |
           ;

sequence: ADDSEQUENCE QSTRING
          sequence_options { sequence.setName( $2 ); db.addSequence( sequence ); sequence.reset(); }
          ;

sequence_options: sequence_options sequence_option
                  | sequence_option
                  ;

sequence_option: INITIALVAL INT { sequence.setInitial( $2 ); }
                 | INCREMENT INT { sequence.setIncrement( $2 ); }
                 | CYCLEONLIMIT LOGICAL { sequence.setCylceOnLimit( $2 ); }
                 | MINVAL INT { sequence.setMinVal( $2 ); }
                 | MAXVAL INT { sequence.setMaxVal( $2 ); }
                 ;

tables: tables tabledef
        | tabledef
        ;

tabledef: ADDTABLE QSTRING
          table_options
          fields indexes { table.setName( $2 ); table.sortFields(); db.addTable( table ); table.reset(); }
          ;

table_options: table_options table_option
               | table_option
               ;

table_option: AREA QSTRING
              | DESCRIPTION nested_qstring { table.setDescription( $2 ); }
              | DUMPNAME QSTRING { table.setDumpName( $2 ); }
              | LABEL QSTRING { table.setLabel( $2 ); }
              | VALEXP nested_qstring { table.setValexp( $2 ); }
              | VALMSG QSTRING { table.setValmsg( $2 ); }
              | FROZEN
              | HIDDEN
              | CATEGORY QSTRING
              ;

fields: fields field 
        | field 
   ;

field: ADDFIELD QSTRING OF QSTRING AS DATATYPE 
       field_options { field.setName( $2 ); field.setTable( $4 ); field.setType( $6 ); table.addField( field ); field.reset(); }
       ;

field_options: field_options field_option 
               | field_option
               ;

field_option: DESCRIPTION nested_qstring { field.setDescription( $2 ); }
              | INITIALVAL strornull { field.setInitial( $2 ); }
              | FORMAT QSTRING { field.setFormat( $2 ); }
              | MANDATORY { field.setMandatory(); }
              | LABEL QSTRING { field.setLabel( $2 ); }
              | POSITION INT { field.setPosition( $2 ); }
              | MAXWIDTH INT { field.setLength( $2 ); }
              | COLUMNLABEL QSTRING { field.setColumnLabel( $2 ); }
              | ORDER INT { field.setOrder( $2 ); }
              | HELP nested_qstring { field.setHelp( $2 ); }
              | VALEXP nested_qstring { field.setValexp( $2 ); }
              | VALMSG QSTRING { field.setValmsg( $2 ); }
              | EXTENT INT { field.setExtent( $2 ); }
              | DECIMALS INT { field.setDecimal( $2 ); }
              | CASESENSITIVE { field.setCaseSensitive(); }
              ;

nested_qstring: nested_qstring QSTRING
                | QSTRING
                ;
               
strornull: QSTRING
           | DBNULL
           ;

indexes: indexes index
         | index
         ;

index: ADDINDEX QSTRING ON QSTRING
       optional_area
       index_options
       index_fields { index.setName( $2 ); index.setTable( $4 ); table.addIndex( index ); index.reset(); }
       ;

optional_area: AREA QSTRING { index.setArea( $2 ); };
               |
               ;

index_options: index_options index_option
               | index_option
               ;

index_option: UNIQUE { index.setUnique(); }
              | PRIMARY { index.setPrimary(); }
              | WORD { index.setWord(); }
              |
              ;

index_fields: index_fields index_field
              | index_field
              ;

index_field: INDEXFIELD QSTRING direction { index.addIndexField( $2, index_direction );  }
             ;

direction: ASCENDING optional_abbrev { index_direction = true; }
           | DESCENDING optional_abbrev { index_direction = false; }
           ;

optional_abbrev: ABBREVIATED
                 |
                 ;

%%
