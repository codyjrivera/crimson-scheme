#include <iostream>
#include <string>
#include "Exp.hpp"
#include "Lexer.hpp"
#include "Token.hpp"
#include "Error.hpp"
#include "Interpreter.hpp"


// Implementation of top-level interpreter class

Interpreter::Interpreter()
  : lexer(Lexer()),
    output(&std::cout),
    aux(&std::cout) {}

Interpreter::~Interpreter() {}

Interpreter::Interpreter(const Interpreter& ip)
  : lexer(ip.lexer),
    output(ip.output),
    aux(ip.aux) {}

Interpreter& operator=(const Interpreter& ip)
{
  lexer = ip.lexer;
  output = ip.output;
  aux = ip.aux;
  return *this;
}

