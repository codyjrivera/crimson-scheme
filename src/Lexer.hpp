#ifndef Lexer_HPP
#define Lexer_HPP

#include <istream>
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
  TokenIterator getTokens();
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
