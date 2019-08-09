#ifndef _Parser_HPP
#define _Parser_HPP

#include <iostream>
#include <string>
#include <list>
#include <exception>
#include "Lexer.hpp"
#include "Data.hpp"
#include "Exp.hpp"
#include "Error.hpp"


class Parser
{
private:
    std::vector<Exp*> allocNodes;
    Exp* parsePrimitive(Lexer& lex);
    Exp* parseNumber(Lexer& lex);
    Exp* parseQuote(Lexer& lex);
    Exp* parseHash(Lexer& lex);
    Exp* parseSExp(Lexer& lex);
    Exp* parseCompoundExp(Lexer& lex);
    Exp* parseCompoundRest(Lexer& lex);

    
public:
    // Parsing routines
    Exp* parseExp(Lexer& lex);
    Exp* parseFile(Lexer& lex);
};


#endif
