#ifndef _Lexer_HPP
#define _Lexer_HPP


#include <iostream>
#include "Token.hpp"


class Lexer
{
private:
  std::istream* inStream;
  long long line;
  long long col;

  Token nextToken;
  bool saveToken;
  Token lexToken();
  
public:
  Lexer();
  Lexer(std::istream& stream);
  ~Lexer();
  Lexer(const Lexer& lex);
  Lexer& operator=(const Lexer& lex);
  void setStream(std::istream& stream);
  void resetPosition();
  Token peek();
  Token next();
  long long getLine();
  long long getCol();
};

#endif
