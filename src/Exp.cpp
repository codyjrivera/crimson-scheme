#include <iostream>
#include <string>
#include <memory>
#include <exception>
#include "Lexer.hpp"
#include "Error.hpp"
#include "Exp.hpp"


// Common Expression Routines
bool Exp::isMarked()
{
  return markVal;
}

void Exp::setMark(bool value)
{
  markVal = value;
}

// Top level expression implementation
TopExp::TopExp()
  : program(new NoneExp()) {}

TopExp::~TopExp() {}

TopExp::TopExp(const TopExp& exp)
{
  markVal = exp.markVal;
  line = exp.line;
  col = exp.col;
  program = std::move(exp.program->clone());
}

TopExp& TopExp::operator=(const TopExp& exp)
{
  markVal = exp.markVal;
  line = exp.line;
  col = exp.col;
  program = std::move(exp.program->clone());
  return *this;
}

std::unique_ptr<Exp> TopExp::clone()
{
  return std::unique_ptr<Exp>(new TopExp(*this));
}

void TopExp::mark()
{
  if (!program->isMarked())
    program->mark();
}

ExpType TopExp::getType()
{
  return ExpType::TOP;
}

void TopExp::applyProduction(Lexer& lexer)
{
  program = std::move(Exp::parse(lexer));
}

Exp& TopExp::eval(Env& env)
{
  Exp& lastExp = program->select(env);
  return lastExp.eval(env);
}

Exp& TopExp::select(Env& env)
{
  return program->select(env);
}

void TopExp::print(std::ostream& stream)
{
  stream << "<TOP-LEVEL EXPRESSION>" << std::endl;
  program->print(stream);
  stream << std::endl << "<END TOP-LEVEL EXPRESSION>" << std::endl;
}

void TopExp::accept(Visitor& vis)
{
  vis.visit(*this);
}

void TopExp::parseExp(Lexer& lexer)
{
  applyProduction(lexer);
}

void TopExp::parseExps(Lexer& lexer)
{
  (void) lexer;
  throw new ParseError("Interpreter does not support parsing multiple top-level exps at this time");
}



// None expression implementation
std::unique_ptr<Exp> NoneExp::clone()
{
  return std::unique_ptr<Exp>(new NoneExp(*this));
}

void NoneExp::mark()
{
  setMark(true);
}

ExpType NoneExp::getType()
{
  return ExpType::NONE;
}

void NoneExp::applyProduction(Lexer& lexer)
{
  (void) lexer;
}

Exp& NoneExp::eval(Env& env)
{
  (void) env;
  return *this;
}

Exp& NoneExp::select(Env& env)
{
  (void) env;
  return *this;
}

void NoneExp::print(std::ostream& stream)
{
  stream << "";
}

void NoneExp::accept(Visitor& vis)
{
  vis.visit(*this);
}


