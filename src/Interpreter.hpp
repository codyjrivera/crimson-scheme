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


class Interpreter
{
private:
    Exp* program;
    std::list<Env*> envList;
    // To add - procList, perhaps symList, definitely heap
    Data result;
  
    std::ostream *output;
    std::ostream *aux;

    Data eval(Exp& exp, Env& env);
    // TODO -- Add Eval when needed
  
public:
    Interpreter(std::ostream& o = std::cout, std::ostream& a = std::cout) : output(&o), aux(&a)
    {
        initInterpreter();
    }

    ~Interpreter();

    void initInterpreter();
    void parseExp(Lexer& lex);
    void parseFile(Lexer& lex);
    void eval();
    std::string resultToString();
    
    void setOutput(std::ostream& stream);
    void setAux(std::ostream& stream);
};

#endif
