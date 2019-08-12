#include <iostream>
#include <string>
#include <vector>
#include <list>
#include "Exp.hpp"
#include "Env.hpp"
#include "Data.hpp"
#include "Lexer.hpp"
#include "Token.hpp"
#include "Error.hpp"
#include "Interpreter.hpp"






// Evaluation routine
Data Interpreter::eval(Exp& exp, Env& env)
{
    // To handle tail calls
    Data result;
    Exp* evalExp = &exp;
    Env* evalEnv = &env;
    bool evalFlag = true;
    while (evalFlag)
    {
        evalFlag = false;

        // Evaluation dispatch
        if (evalExp->isData())
        {
            // Primitive Types and Variable Lookup
            if (evalExp->getData().type == DataType::SYMBOL)
            {
                result = evalEnv->lookup(evalExp->getData().text);
            }
            else
            {
                result = evalExp->getData();
            }
        }
        else
        {
            // Language Primitives
            
            throw InterpreterError("Not an Expression", evalExp->getLine(), evalExp->getCol());
        }
    }
    return result;
}

