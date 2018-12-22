#ifndef _Token_HPP
#define _Token_HPP

/* Tokens are stored as strings in a class Token, where it is
   bundled along with a TokenType */

#include <string>

enum TokenType
{
  PAREN,
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
  std::string value;
  long long line, col;
public:
  // Essential member functions
  Token();
  Token(TokenType type, std::string val, long long l, long long c);
  ~Token();
  Token(const Token& tok);
  Token& operator=(const Token& tok);
  // Getters and setters
  TokenType getType();
  void setType(TokenType type);
  std::string getValue();
  void setValue(std::string str);
  long long getLine();
  void setLine(long long val);
  long long getCol();
  void setCol(long long val);
};


#endif
