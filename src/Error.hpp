#ifndef _Error_HPP
#define _Error_HPP

#include <iostream>
#include <string>
#include <exception>

class InterpreterError
{
public:
  virtual long long getLine() = 0;
  virtual long long getCol() = 0;
  virtual void print(std::ostream& stream) = 0;
};


// Implements InterpreterError, based on std::runtime_error
class ParseError : public InterpreterError, public std::runtime_error
{
private:
  long long line, col;
public:
  // 
  ParseError(const std::string& error);
  ParseError(const std::string& error, long long l, long long c);
  ~ParseError();
  long long getLine() override;
  void setLine(long long l);
  long long getCol() override;
  void setCol(long long c);
  void print(std::ostream& stream);
};


#endif
