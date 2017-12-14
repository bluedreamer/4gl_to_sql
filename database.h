#ifndef DATABASE_H
#define DATABASE_H

#include <istream>
#include <ostream>
#include <vector>
#include "table.h"
#include "sequence.h"

class Database
{
   public:
      typedef std::vector<Table> Tables;
      typedef std::vector<Sequence> Sequences;
      Database() {}
      friend std::ostream &operator<<( std::ostream &os, const Database &db );

      void convertDumpToSql();
      void addTable( const Table &table );
      void addSequence( const Sequence &sequence );
      std::string dumpIndexesAsAlterTable() const;
   private:
      Tables tables_;
      Sequences sequences_;
};

extern Database db;

#endif
