#ifndef _Lexer_HPP
#define _Lexer_HPP


#include <iostream>
#include "Token.hpp"


class Lexer
{
public:
  Lexer();
  ~Lexer();
  Token lexToken();
};

#endif
