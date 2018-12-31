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

Interpreter& Interpreter::operator=(const Interpreter& ip)
{
  lexer = ip.lexer;
  output = ip.output;
  aux = ip.aux;
  return *this;
}

// Operations

void Interpreter::runFile(std::istream& stream)
{
  (void)stream;
  *output << "runFile not implemented yet" << std::endl;
}

void Interpreter::repl(std::istream& stream)
{
  Token tok;
  lexer.setStream(stream);
  tok = lexer.next();
  while (tok.getType() != TokenType::END)
  {
    tok.print(*output);
    tok = lexer.next();
  }
  tok.print(*output);
}

void Interpreter::setOutput(std::ostream& stream)
{
  output = &stream;
}

void Interpreter::setAux(std::ostream& stream)
{
  aux = &stream;
}
