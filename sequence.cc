#include <sstream>
#include "sequence.h"


void Sequence::reset()
{
   name_.clear();
   initial_.reset();
   increment_.reset();
   cycleOnLimit_.reset();
   minVal_.reset();
   maxVal_.reset();
}

std::string Sequence::schemaName_ = "INFO_sequences";

std::string Sequence::schema()
{
   std::ostringstream strm;
   strm << "DROP TABLE IF EXISTS " << schemaName_ << ";\n";
   strm << "CREATE TABLE " << schemaName_ << "\n(\n";
   strm << "name VARCHAR(50),\n";
   strm << "PRIMARY KEY (name),\n";
   strm << "initial INT,\n";
   strm << "increment INT,\n";
   strm << "cycleonlimit TINYINT(1),\n";
   strm << "minval INT,\n";
   strm << "maxval INT\n";
   strm << ") ENGINE=InnoDB COMMENT = 'Add sequence records from Dump File';\n";
   return strm.str();
}

std::string Sequence::insertStatement() const
{
   std::ostringstream strm;
   strm << "INSERT INTO `" << schemaName_ << "` (name, initial, increment, cycleonlimit, minval, maxval) ";
   strm << "VALUES(";
   strm << '\'' << name_ << "',";
   printOptional(strm, initial_);
   printOptional(strm, increment_);
   printOptional(strm, cycleOnLimit_);
   printOptional(strm, minVal_);
   printOptional(strm, maxVal_, false);
   strm << ");\n";
   return strm.str();
}

void Sequence::setName(const std::string &s)
{
   name_ = trimquote(s);
}

void Sequence::setInitial(int i)
{
   initial_ = i;
}

void Sequence::setIncrement(int i)
{
   increment_ = i;
}

void Sequence::setCylceOnLimit(bool b)
{
   cycleOnLimit_ = b;
}

void Sequence::setMinVal(int i)
{
   minVal_ = i;
}

void Sequence::setMaxVal(int i)
{
   maxVal_ = i;
}

