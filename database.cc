#include <sstream>
#include "database.h"

std::ostream &operator<<( std::ostream &os, const Database &db )
{
   // Dump table defs
   for( Database::Tables::const_iterator i=db.tables_.begin(), end=db.tables_.end(); i!=end; ++i)
   {
      os << (*i);
   }
   // Dump foreign keys
   for( Database::Tables::const_iterator i=db.tables_.begin(), end=db.tables_.end(); i!=end; ++i)
   {
//      os << i->getForeignKeysDefs();
   }
   os << Sequence::schema();
   os << "BEGIN;\n";
   for( Database::Sequences::const_iterator i=db.sequences_.begin(), end=db.sequences_.end(); i!=end; ++i)
   {
//      os << i->insertStatement();
   }
   os << "COMMIT;\n";

   os << Table::schema();
   os << "BEGIN;\n";
   for( Database::Tables::const_iterator i=db.tables_.begin(), end=db.tables_.end(); i!=end; ++i)
   {
//      os << i->insertStatement();
   }
   os << "COMMIT;\n";

   os << Field::schema();
   os << "BEGIN;\n";
   for( Database::Tables::const_iterator i=db.tables_.begin(), end=db.tables_.end(); i!=end; ++i)
   {
//      os << i->fieldinsertStatement();
   }
   os << "COMMIT;\n";
   return os;
}

void Database::convertDumpToSql()
{
   for( Tables::iterator i=tables_.begin(), end=tables_.end(); i!=end; ++i)
   {
      i->convertDumpToSql();
   }
}

std::string Database::dumpIndexesAsAlterTable() const
{
   std::ostringstream strm;
   for( Tables::const_iterator i=tables_.begin(), end=tables_.end(); i!=end; ++i)
   {
      strm << i->dumpIndexesAsAlterTable();
   }
   return strm.str();
}
