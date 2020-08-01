#ifndef _Parser_HPP
#define _Parser_HPP

#include <exception>
#include <iostream>
#include <list>
#include <string>

#include "Data.hpp"
#include "Error.hpp"
#include "Exp.hpp"
#include "Lexer.hpp"

class Parser {
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
