#ifndef UTIL_H
#define UTIL_H

#include <string>

void trim( std::string &str );
std::string trimquote( const std::string &str );
void escapestring( std::string &str );
void tolower( std::string &str );
void replacehypens( std::string &str );

#endif
