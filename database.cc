#include <sstream>
#include "database.h"

std::ostream &operator<<(std::ostream &os, const Database &db)
{
   // Dump table defs
   for(auto &i : db.tables_)
   {
      os << i;
   }
   // Dump foreign keys
   for(auto &i : db.tables_)
   {
      os << i.getForeignKeysDefs();
   }
   os << Sequence::schema();
   os << "BEGIN;\n";
   for(auto &i : db.sequences_)
   {
      os << i.insertStatement();
   }
   os << "COMMIT;\n";

   os << Table::schema();
   os << "BEGIN;\n";
   for(auto &i : db.tables_)
   {
      os << i.insertStatement();
   }
   os << "COMMIT;\n";

   os << Field::schema();
   os << "BEGIN;\n";
   for(auto &i : db.tables_)
   {
      os << i.fieldinsertStatement();
   }
   os << "COMMIT;\n";
   return os;
}

void Database::convertDumpToSql()
{
   for(auto &i : tables_)
   {
      i.convertDumpToSql();
   }
}

std::string Database::dumpIndexesAsAlterTable() const
{
   std::ostringstream strm;
   for(auto &i : tables_)
   {
      strm << i.dumpIndexesAsAlterTable();
   }
   return strm.str();
}

void Database::addTable(Table table)
{
   tables_.emplace_back(table);
}

void Database::addSequence(Sequence sequence)
{
   sequences_.emplace_back(std::move(sequence));
}
