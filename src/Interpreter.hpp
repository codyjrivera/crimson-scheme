#ifndef _Interpeter_HPP
#define _Interpeter_HPP


#include <iostream>
#include <string>
#include <list>
#include "Exp.hpp"
#include "Env.hpp"
#include "Data.hpp"
#include "Lexer.hpp"
#include "Token.hpp"
#include "Error.hpp"


/*
  NOTE - this class is implemented in several modules, as this class has
  several concerns, listed here

  Evaluation is handled in Eval.cpp
  
  Language Primitives and Run-Time Support - Language.cpp
  Pretty-Printing and Result Printing, Misc. - Interpreter.cpp

  Other concerns are handled in member classes
 */


class Interpreter
{
private:
    Exp* program;
    Env topEnv;
    Data result;

    std::istream *input;
    std::ostream *output;
    std::ostream *aux;

    Exp* parseExp(Lexer& lex);
    Exp* parseMultipleExps(Lexer& lex);
    std::string prettyPrint(Exp& exp);

    // Implementations of Evaluation in Eval.cpp
    Data eval(Exp* exp, Env& env);
    Data evalDefine(Exp* exp, Env& env);
    Data evalSet(Exp* exp, Env& env);
    Exp* evalIf(Exp* exp, Env& env);
    Data evalWhile(Exp* exp, Env& env);
    Exp* evalBegin(Exp* exp, Env& env);
    // TODO -- Add Apply when needed
    std::string dataToString(Data& result);
    // Here because sometimes printing requires additional context
    // that isn't provided by the Data class itself
    
  
public:
    Interpreter(std::istream& i = std::cin, std::ostream& o = std::cout, std::ostream& a = std::cout) : program(NULL), input(&i), output(&o), aux(&a)
    {
        initInterpreter();
    }

    ~Interpreter();

    void initInterpreter();
    void readExp(Lexer& lex);
    void readFile(Lexer& lex);
    std::string programToString();
    void eval();
    std::string resultToString();
    
    void setOutput(std::ostream& stream);
    void setAux(std::ostream& stream);

    // Primitive Friend Functions
    friend void primDisplay(Data& result, std::vector<Data>& args, Interpreter& interpreter);
    friend void primNewline(Data& result, std::vector<Data>& args, Interpreter& interpreter);
    
};

#endif
