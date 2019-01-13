#include <iostream>
#include <string>
#include <limits>
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
  *aux << "Crimson-Scheme REPL:" << std::endl;
  *aux << "Press CTRL-D to exit" << std::endl << std::endl;
  while (!stream.eof())
  {
    *aux << "> ";
    try
    {
      program.parseExp(lexer);
      program.eval(topEnv).print(*output);
    }
    catch (InterpreterError& e)
    {
      e.print(*aux);
      // Clears out REPL to handle next line
      stream.clear();
      stream.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    }
    lexer.resetPosition();
    *aux << std::endl;
  }
}

void Interpreter::setOutput(std::ostream& stream)
{
  output = &stream;
}

void Interpreter::setAux(std::ostream& stream)
{
  aux = &stream;
}
