#ifndef _AST_HPP
#define _AST_HPP

#include <iostream>
#include <string>
#include <memory>
#include <exception>
#include "Token.hpp"
#include "Lexer.hpp"
#include "Error.hpp"

class AST
{
private:
  std::unique_ptr<AST> contents;
  std::unique_ptr<AST> next;
public:
  AST();
  ~AST();
  AST(const AST& node);
  AST& operator=(const AST& node);
  AST& getContents();
  AST& getNext();
  void setContents(AST& node);
  void setNext(AST& node);
  void clear();
};


class ParseError : virtual public InterpreterError, std::runtime_error
{
private:
  long long line;
  long long col;
public:
  ParseError(const std::string& what);
  ParseError(const std::string& what, long long l, long long c);
  long long getLine() override;
  long long getCol() override;
  void print(std::ostream& stream) override;
};


#endif
