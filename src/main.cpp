/* 
   main.c -- by Cody Rivera
   Copyright 2018 Cody Rivera
   This program is distributed under the terms of the GNU GPL, version 3
   or later. You should have recieved a copy of this license with this 
   repository.

   cri-scm [file] [options]
   -v or --version -- Prints the version and quits
   -b or --batch -- Runs the file and quits
 */

#include <iostream>
#include <fstream>
#include <string>
#include "Lexer.hpp"
#include "Token.hpp"

int main()
{
  Lexer lex;
  Token tok;
  tok = lex.nextToken();
  while (tok.getType() != END)
  {
    tok.printToken(std::cout);
    tok = lex.nextToken();
  }
  tok.printToken(std::cout);
  return 0;
}
