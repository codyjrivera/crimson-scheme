#include <iostream>
#include <string>
#include <exception>
#include "Exp.hpp"
#include "Error.hpp"


// Implementation of ParseError

ParseError::ParseError(const std::string& error)
  : std::runtime_error(error),
    line(0),
    col(0) {}

ParseError::ParseError(const std::string& error, long long l, long long c)
  : std::runtime_error(error),
    line(l),
    col(c) {}

ParseError::~ParseError() {}


long long ParseError::getLine()
{
  return line;
}

void ParseError::setLine(long long l)
{
  line = l;
}

long long ParseError::getCol()
{
  return col;
}

void ParseError::setCol(long long c)
{
  col = c;
}


void ParseError::print(std::ostream& stream)
{
  stream << std::endl;
  stream << "At line: " << line << ", col: " << col << std::endl;
  stream << std::endl;
  stream << "Parse Error: " << what() << std::endl;
  stream << std::endl;
}

