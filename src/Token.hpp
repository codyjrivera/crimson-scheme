#ifndef _Token_HPP
#define _Token_HPP

/* Tokens are stored as strings in a class Token, where it is
   bundled along with a TokenType */

#include <iostream>
#include <string>

enum class TokenType
{
    PAREN,
    DOT,
    QUOTE,
    HASH,
    IDENTIFIER,
    BOOLEAN,
    NUMBER,
    STRING,
    END
};

class Token
{
  private:
    TokenType tokenType;
    std::string value = std::string("");
    long line, col;

  public:
    // Essential member functions
    Token();
    Token(TokenType type, std::string val, long l, long c);
    ~Token();
    Token(const Token& tok);
    Token& operator=(const Token& tok);
    // Getters and setters
    TokenType getType() const;
    void setType(TokenType type);
    std::string getValue() const;
    void setValue(std::string str);
    long getLine() const;
    void setLine(long val);
    long getCol() const;
    void setCol(long val);
    std::string toString() const;
};


std::ostream& operator<<(std::ostream& stream, const Token& tok);


#endif
