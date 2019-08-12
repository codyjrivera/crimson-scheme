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
#include "Interpreter.hpp"


void runFile(std::istream& file);
void repl(Interpreter& i);


int main()
{
    Interpreter main;
    repl(main);
    return 0;
}


void repl(Interpreter& i)
{
    Lexer lexer(std::cin);
    std::cout << "Crimson-Scheme REPL:" << std::endl;
    std::cout << "Press CTRL-D to exit" << std::endl << std::endl;
    while (!std::cin.eof())
    {
        std::cout << "> ";
        try
        {
            i.readExp(lexer);
            i.eval();
            std::cout << i.resultToString() << std::endl;
        }
        catch (InterpreterError& e)
        {
            std::cout << std::endl << e;
        }
        lexer.flush();
        std::cout << std::endl;
    }
}





