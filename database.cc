#include <sstream>
#include "database.h"

std::ostream &operator<<( std::ostream &os, const Database &db )
{
   // Dump table defs
   for( auto i : db)
   {
      os << (*i);
   }
   // Dump foreign keys
   for( auto i : db)
   {
//      os << i->getForeignKeysDefs();
   }
   os << Sequence::schema();
   os << "BEGIN;\n";
   for( auto i : db)
   {
//      os << i->insertStatement();
   }
   os << "COMMIT;\n";

   os << Table::schema();
   os << "BEGIN;\n";
   for(auto i : db)
   {
//      os << i->insertStatement();
   }
   os << "COMMIT;\n";

   os << Field::schema();
   os << "BEGIN;\n";
   for(auto i : db)
   {
//      os << i->fieldinsertStatement();
   }
   os << "COMMIT;\n";
   return os;
}

void Database::convertDumpToSql()
{
   for(auto i : tables_)
   {
      i->convertDumpToSql();
   }
}

std::string Database::dumpIndexesAsAlterTable() const
{
   std::ostringstream strm;
   for(auto i : tables_)
   {
      strm << i->dumpIndexesAsAlterTable();
   }
   return strm.str();
}

void Database::addTable(const Table &table)
{
   tables_.push_back(table);
}

void Database::addSequence(const Sequence &sequence)
{
   sequences_.push_back(sequence);
}
