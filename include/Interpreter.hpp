#ifndef _Interpreter_HPP
#define _Interpreter_HPP


#include "Parser.h"
#include "AST.h"
#include "Value.h"
#include "Store.h"
#include "Environment.h"
#include "Execution.h"
#include "Printer.h"


class Interpreter
{
private:
  Parser input;
  AST program;
  Store data;
  Environment globalEnv;
  Execution globalExec;
  Printer output;
  
public:
  Interpreter();
  Interpreter(istream& stream);
  ~Interpreter();
  void setInput(istream& stream);
  void setOutput(ostream& stream);
  void evalStatement();
  Value eval(AST& exp, Environment env, Execution globalExec);
};


#endif
