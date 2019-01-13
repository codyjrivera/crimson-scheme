#include <iostream>
#include <string>
#include <memory>
#include <exception>
#include "Lexer.hpp"
#include "Error.hpp"
#include "Exp.hpp"
#include "Data.hpp"


// Implementation of Boolean
Boolean::Boolean(bool value)
  : objValue(value) {}

std::unique_ptr<Exp> Boolean::clone()
{
  return std::unique_ptr<Exp>(new Boolean(*this));
}

ExpType Boolean::getType()
{
  return ExpType::BOOLEAN;
}

void Boolean::applyProduction(Lexer& lexer)
{
  (void) lexer;
}

Exp& Boolean::eval(Env& env)
{
  (void) env;
  return *this;
}

Exp& Boolean::select(Env& env)
{
  (void) env;
  return *this;
}

void Boolean::print(std::ostream& stream)
{
  if (objValue)
    stream << "#t";
  else
    stream << "#f";
}

void Boolean::accept(Visitor& vis)
{
  vis.visit(*this);
}

bool Boolean::getValue()
{
  return objValue;
}

void Boolean::setValue(bool value)
{
  objValue = value;
}



// Implementation of Integer
Integer::Integer(long long value)
  : objValue(value) {}

std::unique_ptr<Exp> Integer::clone()
{
  return std::unique_ptr<Exp>(new Integer(*this));
}

ExpType Integer::getType()
{
  return ExpType::INTEGER;
}

void Integer::applyProduction(Lexer& lexer)
{
  (void) lexer;
}

Exp& Integer::eval(Env& env)
{
  (void) env;
  return *this;
}

Exp& Integer::select(Env& env)
{
  (void) env;
  return *this;
}

void Integer::print(std::ostream& stream)
{
  stream << objValue;
}

void Integer::accept(Visitor& vis)
{
  vis.visit(*this);
}

long long Integer::getValue()
{
  return objValue;
}

void Integer::setValue(long long value)
{
  objValue = value;
}



// Implementation of Real
Real::Real(double value)
  : objValue(value) {}

std::unique_ptr<Exp> Real::clone()
{
  return std::unique_ptr<Exp>(new Real(*this));
}

ExpType Real::getType()
{
  return ExpType::REAL;
}

void Real::applyProduction(Lexer& lexer)
{
  (void) lexer;
}

Exp& Real::eval(Env& env)
{
  (void) env;
  return *this;
}

Exp& Real::select(Env& env)
{
  (void) env;
  return *this;
}

void Real::print(std::ostream& stream)
{
  stream << objValue;
}

void Real::accept(Visitor& vis)
{
  vis.visit(*this);
}

double Real::getValue()
{
  return objValue;
}

void Real::setValue(double value)
{
  objValue = value;
}



// Implementation of String
String::String(std::string value)
  : objValue(value) {}

std::unique_ptr<Exp> String::clone()
{
  return std::unique_ptr<Exp>(new String(*this));
}

ExpType String::getType()
{
  return ExpType::STRING;
}

void String::applyProduction(Lexer& lexer)
{
  (void) lexer;
}

Exp& String::eval(Env& env)
{
  (void) env;
  return *this;
}

Exp& String::select(Env& env)
{
  (void) env;
  return *this;
}

void String::print(std::ostream& stream)
{
  stream << "\"" << objValue << "\"";
}

void String::accept(Visitor& vis)
{
  vis.visit(*this);
}

std::string String::getValue()
{
  return objValue;
}

void String::setValue(std::string value)
{
  objValue = value;
}

std::string& String::getRef()
{
  return objValue;
}




// Implementation of Symbol
Symbol::Symbol(std::string value)
  : objValue(value) {}

std::unique_ptr<Exp> Symbol::clone()
{
  return std::unique_ptr<Exp>(new Symbol(*this));
}

ExpType Symbol::getType()
{
  return ExpType::SYMBOL;
}

void Symbol::applyProduction(Lexer& lexer)
{
  (void) lexer;
}

Exp& Symbol::eval(Env& env)
{
  (void) env;
  return *this;
}

Exp& Symbol::select(Env& env)
{
  (void) env;
  return *this;
}

void Symbol::print(std::ostream& stream)
{
  stream << objValue;
}

void Symbol::accept(Visitor& vis)
{
  vis.visit(*this);
}

std::string Symbol::getValue()
{
  return objValue;
}

void Symbol::setValue(std::string value)
{
  objValue = value;
}

std::string& Symbol::getRef()
{
  return objValue;
}

