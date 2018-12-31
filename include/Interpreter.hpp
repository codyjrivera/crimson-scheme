#ifndef _Interpeter_HPP
#define _Interpeter_HPP


#include <iostream>
#include <string>
#include "Exp.hpp"
#include "Lexer.hpp"
#include "Token.hpp"
#include "Error.hpp"


class Interpreter
{
private:
  Lexer lexer;
  std::ostream *output;
  std::ostream *aux;
  
public:
  Interpreter();
  ~Interpreter();
  Interpreter(const Interpreter& ip);
  Interpreter& operator=(const Interpreter& ip);
  void runFile(std::istream& stream);
  void repl(std::istream& stream);
  void setOutput(std::ostream& stream);
  void setAux(std::ostream& stream);
};

#endif
