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
class Env;

class Exp
{
private:
  long long line, col;
public:
  // Most operations virtual, as subclasses have variety of behaviors 
  virtual std::unique_ptr<Exp> clone();
  // For Error Reporting, in superclass because behavior is the same
  long long getLine();
  void setLine(long long l);
  long long getCol();
  void setCol(long long c);
  // Core functions of evaluator
  virtual ExpType getType() = 0;
  virtual void applyProduction(Lexer& lexer) = 0;
  static std::unique_ptr<Exp> parse(Lexer& lexer);
  static std::unique_ptr<Exp> parseData(Lexer& lexer);
  virtual Exp& eval(Env& env) = 0;
  // Select brings unevaled code down a few stack levels to preserve
  // proper tail calls
  virtual Exp& select(Env& env) = 0;
  virtual void print(std::ostream& stream) = 0;
  // Visitor
  virtual void accept(Visitor& vis) = 0;
};


class TopExp : public Exp
{
private:
  std::unique_ptr<Exp> program;
public:
  // Constructors, copy constructors, and destructors
  TopExp();
  ~TopExp();
  TopExp(const TopExp& exp);
  TopExp& operator=(const TopExp& exp);
  // Virtual functions declared in Exp
  std::unique_ptr<Exp> clone();
  ExpType getType() override;
  void applyProduction(Lexer& lexer) override;
  Exp& eval(Env& env) override;
  Exp& select(Env& env) override;
  void print(std::ostream& stream) override;
  void accept(Visitor& vis) override;
  // Unique functions
  void parseExp(Lexer& lexer);
  void parseExps(Lexer& lexer);
};

class Env : public Exp
{
};

// Virtual Visitor methods for each derived expression class
class Visitor
{
  virtual void visit(TopExp& exp);
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
