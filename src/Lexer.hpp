#ifndef _Lexer_HPP
#define _Lexer_HPP

#include <iostream>
#include <sstream>
#include <string>

#include "Token.hpp"

class Lexer {
   private:
    std::istream* inStream;
    long line;
    long col;

    Token nextToken;
    bool saveToken;
    Token lexToken();

   public:
    Lexer();
    Lexer(std::istream& stream);
    ~Lexer();
    Lexer(const Lexer& lex);
    Lexer& operator=(const Lexer& lex);
    void setStream(std::istream& stream);
    void flush();
    Token peek();
    Token next();
    long getLine();
    long getCol();
};

#endif
