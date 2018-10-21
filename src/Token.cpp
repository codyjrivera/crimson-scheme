/* Implementation of functions in class Token */

#include <string>
#include "Token.hpp"


// Placeholder constructor and destructor
Token::Token(){}
Token::~Token(){}

// Copy and Assignment Constructors
Token::Token(const Token& tok) : tokenType(tok.tokenType),
                                 token(tok.token) {}

Token& Token::operator=(const Token& tok)
{
  tokenType = tok.tokenType;
  token = tok.token;
  return *this;
}

// Getters and setters
TokenType Token::getType()
{
  return tokenType;
}

void Token::setType(TokenType type)
{
  tokenType = type;
}

std::string Token::getToken()
{
  return token;
}

void Token::setToken(std::string str)
{
  token = str;
}

long long getLine()
{
  return line;
}

void setLine(long long val)
{
  line = val;
}

long long getCol()
{
  return col;
}

void setCol(long long val)
{
  col = val;
}
