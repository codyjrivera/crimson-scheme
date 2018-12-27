#ifndef _Error_HPP
#define _Error_HPP

#include <iostream>
#include <exception>

class InterpreterError
{
public:
  virtual long long getLine() = 0;
  virtual long long getCol() = 0;
  virtual void print(std::ostream& stream) = 0;
};


#endif
