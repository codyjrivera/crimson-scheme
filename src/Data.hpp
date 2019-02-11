#ifndef _Data_HPP
#define _Data_HPP

#include <iostream>
#include <string>
#include <memory>
#include <exception>
#include "Lexer.hpp"
#include "Error.hpp"
#include "Exp.hpp"


// Boolean data - either true or false
class Boolean : public Exp
{
private:
  bool objValue;
public:
  Boolean() = default;
  Boolean(bool value);
  ~Boolean() = default;
  Boolean(const Boolean& value) = default;
  Boolean& operator=(const Boolean& value) = default;
  // Virtual functions in Exp
  std::unique_ptr<Exp> clone() override;
  ExpType getType() override;
  void applyProduction(Lexer& lexer) override;
  Exp& eval(Env& env) override;
  Exp& select(Env& env) override;
  void print(std::ostream& stream) override;
  void accept(Visitor& vis) override;
  // Unique functions
  bool getValue();
  void setValue(bool value);
};


// Integer data - uses long long
class Integer : public Exp
{
private:
  long long objValue;
public:
  Integer() = default;
  Integer(long long value);
  ~Integer() = default;
  Integer(const Integer& value) = default;
  Integer& operator=(const Integer& value) = default;
  // Virtual functions in Exp
  std::unique_ptr<Exp> clone() override;
  ExpType getType() override;
  void applyProduction(Lexer& lexer) override;
  Exp& eval(Env& env) override;
  Exp& select(Env& env) override;
  void print(std::ostream& stream) override;
  void accept(Visitor& vis) override;
  // Unique functions
  long long getValue();
  void setValue(long long value);
};


// Real data - uses double
class Real : public Exp
{
private:
  double objValue;
public:
  Real() = default;
  Real(double value);
  ~Real() = default;
  Real(const Real& value) = default;
  Real& operator=(const Real& value) = default;
  // Virtual functions in Exp
  std::unique_ptr<Exp> clone() override;
  ExpType getType() override;
  void applyProduction(Lexer& lexer) override;
  Exp& eval(Env& env) override;
  Exp& select(Env& env) override;
  void print(std::ostream& stream) override;
  void accept(Visitor& vis) override;
  // Unique functions
  double getValue();
  void setValue(double value);
};


// String data - uses std::string
class String : public Exp
{
private:
  std::string objValue;
public:
  String() = default;
  String(std::string value);
  ~String() = default;
  String(const String& value) = default;
  String& operator=(const String& value) = default;
  // Virtual functions in Exp
  std::unique_ptr<Exp> clone() override;
  ExpType getType() override;
  void applyProduction(Lexer& lexer) override;
  Exp& eval(Env& env) override;
  Exp& select(Env& env) override;
  void print(std::ostream& stream) override;
  void accept(Visitor& vis) override;
  // Unique functions
  std::string getValue();
  void setValue(std::string value);
  std::string& getRef();
};

class Symbol : public Exp
{
private:
  std::string objValue;
public:
  Symbol() = default;
  Symbol(std::string value);
  ~Symbol() = default;
  Symbol(const Symbol& value) = default;
  Symbol& operator=(const Symbol& value) = default;
  // Virtual functions in Exp
  std::unique_ptr<Exp> clone() override;
  ExpType getType() override;
  void applyProduction(Lexer& lexer) override;
  Exp& eval(Env& env) override;
  Exp& select(Env& env) override;
  void print(std::ostream& stream) override;
  void accept(Visitor& vis) override;
  // Unique functions
  std::string getValue();
  void setValue(std::string value);
  std::string& getRef();
};

#endif
