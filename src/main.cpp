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

int main(int argc, char** argv)
{
  Lexer lex;
  std::string infile;
  if (argc != 2)
    return 1;
  infile = argv[1];
  std::ifstream file(infile);
  if (!file.is_open())
    return 1;
  lex.lex(file);
  lex.printTokens(std::cout);
  return 0;
}
