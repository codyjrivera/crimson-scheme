#ifndef _Exp_HPP
#define _Exp_HPP

#include <iostream>
#include <string>
#include <memory>
#include <exception>
#include "Lexer.hpp"
#include "Error.hpp"

enum class ExpType
{
  TOP,
  // Data Expressions
  BOOLEAN,
  INTEGER,
  REAL,
  SYMBOL,
  STRING,
  CONS,
  // Control Expressions
  DEFINE,
  ASSIGN,
  VARIABLE,
  LAMBDA,
  PROCEDURE,
  ENVIRONMENT,
  BEGIN,
  IF,
  THUNK,
  NONE
};


// Needs to be forward declared
class Visitor;

class Exp
{
private:
  long long line, col;
public:
  // Most operations virtual, as subclasses have variety of behaviors 
  Exp();
  virtual ~Exp();
  Exp(const Exp& exp);
  Exp& operator=(const Exp& exp);
  virtual std::unique_ptr<Exp> clone(const Exp& exp);
  // For Error Reporting, in superclass because behavior is the same
  long long getLine();
  void setLine(long long l);
  long long getCol();
  void setCol(long long c);
  // Core functions of evaluator
  virtual ExpType getType();
  virtual Exp& eval() = 0;
  // Select brings unevaled code down a few stack levels to preserve
  // proper tail calls
  virtual Exp& select() = 0;
  virtual void print(std::ostream& stream) = 0;
  virtual void applyProduction(Lexer& lexer) = 0;
  std::unique_ptr<Exp> parse(Lexer& lexer);
  // Visitor
  virtual void accept(Visitor& vis) = 0;
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


class Visitor
{
};


#endif
