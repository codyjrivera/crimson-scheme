
/*
  Interpreter.cpp -- Implementation of the interpreter class auxiliary functions
  
  Written by Cody Rivera

  This file is dedicated to implementing printing and maintenance routines for Interpreter


 */

#include "Interpreter.hpp"
#include "Error.hpp"
#include "Exp.hpp"
#include "Lexer.hpp"
#include "Token.hpp"
#include <iostream>
#include <limits>
#include <string>

Interpreter::~Interpreter()
{
    if (program != NULL)
    {
        program->cleanup();
        delete program;
    }
}

void Interpreter::readExp(Lexer& lex)
{
    if (program != NULL)
    {
        program->cleanup();
        delete program;
        program = NULL;
    }
    program = Exp::parseExp(lex);
    
}

void Interpreter::setOutput(std::ostream& stream) { output = &stream; }

void Interpreter::setAux(std::ostream& stream) { aux = &stream; }
