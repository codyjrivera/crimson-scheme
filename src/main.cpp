/*
   main.c -- by Cody Rivera
   Copyright 2018 Cody Rivera

   Permission is hereby granted, free of charge, to any person obtaining a
   copy of this software and associated documentation files (the "Software"),
   to deal in the Software without restriction, including without limitation
   the rights to use, copy, modify, merge, publish, distribute, sublicense,
   and/or sell copies of the Software, and to permit persons to whom the
   Software is furnished to do so, subject to the following conditions:

   The above copyright notice and this permission notice shall be included in
   all copies or substantial portions of the Software.

   THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
   OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
   FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
   AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
   LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
   FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
   DEALINGS IN THE SOFTWARE.

   cri-scm [file] [options]
   -v or --version -- Prints the version and quits
   -b or --batch -- Runs the file and quits
 */

#include <fstream>
#include <iostream>
#include <string>

#include "Interpreter.hpp"

void runFile(Interpreter& i, std::istream& file);
void repl(Interpreter& i);

int main(int argc, char** argv) {
    Interpreter main;
    if (argc <= 1) {
        repl(main);
    } else {
        std::ifstream sourceFile(argv[1]);
        if (!sourceFile) {
            std::cout << "Invalid file: " << argv[1] << std::endl;
        } else {
            runFile(main, sourceFile);
        }
    }
    return 0;
}

void repl(Interpreter& i) {
    Lexer lexer(std::cin);
    std::cout << "Crimson-Scheme REPL:" << std::endl;
    std::cout << "Press CTRL-D to exit" << std::endl << std::endl;
    while (!std::cin.eof()) {
        std::cout << "> ";
        try {
            i.readExp(lexer);
            i.eval();
            std::cout << i.resultToString() << std::endl;
        } catch (InterpreterError& e) {
            std::cout << std::endl << e;
        }
        lexer.flush();
        std::cout << std::endl;
    }
}

void runFile(Interpreter& i, std::istream& file) {
    Lexer lexer(file);
    try {
        i.readFile(lexer);
        i.eval();
    } catch (InterpreterError& e) {
        std::cout << std::endl << e;
    }
}
