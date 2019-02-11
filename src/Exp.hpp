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
  NIL,
  // Control Expressions
  QUOTE,
  DEFINE,
  ASSIGN,
  VARIABLE,
  LAMBDA,
  PROCEDURE,
  CALL,
  ENVIRONMENT,
  BEGIN,
  IF,
  THUNK,
  NONE
};


// Needs to be forward declared
class Exp;
class TopExp;
class NoneExp;
class Boolean;
class Integer;
class Real;
class Symbol;
class String;
class Env;
class Visitor;

class Exp
{
protected:
  bool markVal = false;
  long long line = 0, col = 0;
public:
  Exp() = default;
  ~Exp() = default;
  Exp(const Exp& exp) = default;
  Exp& operator=(const Exp& exp) = default;
  // Most operations virtual, as subclasses have variety of behaviors 
  virtual std::unique_ptr<Exp> clone() = 0;
  // For garbage collection
  bool isMarked();
  void setMark(bool value);
  virtual void mark();
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
  std::unique_ptr<Exp> clone() override;
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


class NoneExp : public Exp
{
public:
  // Constructors, copy constructors, and destructors
  NoneExp() = default;
  ~NoneExp() = default;
  NoneExp(const NoneExp& exp) = default;
  NoneExp& operator=(const NoneExp& exp) = default;
  // Virtual functions declared in Exp
  std::unique_ptr<Exp> clone() override;
  ExpType getType() override;
  void applyProduction(Lexer& lexer) override;
  Exp& eval(Env& env) override;
  Exp& select(Env& env) override;
  void print(std::ostream& stream) override;
  void accept(Visitor& vis) override;
};

class Env
{
public:
  int a;
private:
  int b;
};


// Virtual Visitor methods for each derived expression class
class Visitor
{
public:
  virtual void visit(TopExp& exp) = 0;
  virtual void visit(NoneExp& exp) = 0;
  virtual void visit(Boolean& exp) = 0;
  virtual void visit(Integer& exp) = 0;
  virtual void visit(Real& exp) = 0;
  virtual void visit(String& exp) = 0;
  virtual void visit(Symbol& exp) = 0;
};


#endif
