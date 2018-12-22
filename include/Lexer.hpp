#ifndef _Lexer_HPP
#define _Lexer_HPP

#include <iostream>
#include <vector>
#include <string>
#include "Token.hpp"

using TokenIterator = std::vector<Token>::iterator;

class Lexer
{
private:
  std::vector<Token> tokens;
public:
  Lexer();
  ~Lexer();
  Lexer(const Lexer& lexer);
  Lexer& operator=(Lexer& lexer);
  void clear();
  void getTokens(TokenIterator& begin, TokenIterator& end);
  void printTokens(std::ostream& stream);
  void lex(std::istream& stream);
};

class LexerException : std::exception
{
private:
  std::string errorString;
public:
  LexerException();
  const char* what();
};

#endif
