/* Implementation of functions in class Token */

#include <string>
#include "Token.hpp"


// Constructors and destructors
Token::Token(){}

Token::Token(TokenType type, std::string val, long long l, long long c)
  : tokenType(type),
    value(val),
    line(l),
    col(c) {}

Token::~Token(){}

// Copy and Assignment Constructors
Token::Token(const Token& tok)
  : tokenType(tok.tokenType),
    value(tok.value),
    line(tok.line),
    col(tok.col) {}

Token& Token::operator=(const Token& tok)
{
  tokenType = tok.tokenType;
  value = tok.value;
  line = tok.line;
  col = tok.col;
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

std::string Token::getValue()
{
  return value;
}

void Token::setValue(std::string str)
{
  value = str;
}

long long Token::getLine()
{
  return line;
}

void Token::setLine(long long val)
{
  line = val;
}

long long Token::getCol()
{
  return col;
}

void Token::setCol(long long val)
{
  col = val;
}

void Token::print(std::ostream& stream)
{
  std::string typeString;
  switch (tokenType)
  {
  case TokenType::PAREN:
    typeString = "PAREN";
    break;
  case TokenType::QUOTE:
    typeString = "QUOTE";
    break;
  case TokenType::DOT:
    typeString = "DOT";
    break;
  case TokenType::HASH:
    typeString = "HASH";
    break;
  case TokenType::IDENTIFIER:
    typeString = "IDENTIFIER";
    break;
  case TokenType::BOOLEAN:
    typeString = "BOOLEAN";
    break;
  case TokenType::NUMBER:
    typeString = "NUMBER";
    break;
  case TokenType::STRING:
    typeString = "STRING";
    break;
  case TokenType::END:
    typeString = "END";
    break;
  default:
    typeString = "UNSPECIFIED";
  }
  stream << "{ " << typeString << ": " << value << " }" << std::endl;
  stream << "{ line: " << line << " col: " << col << " }" << std::endl;
}
