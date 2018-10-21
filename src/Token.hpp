#ifndef Token_HPP
#define Token_HPP

/* Tokens are stored as strings in a class Token, where it is
   bundled along with a TokenType */

#include <string>

enum TokenType
{
  PAREN,
  QUOTE,
  HASH,
  IDENTIFIER,
  NUMBER,
  STRING
};


class Token
{
private:
  TokenType tokenType;
  std::string token;
  long long line, col;
public:
  // Essential member functions
  Token();
  ~Token();
  Token(const Token& tok);
  Token& operator=(const Token& tok);
  // Getters and setters
  TokenType getType();
  void setType(TokenType type);
  std::string getToken();
  void setToken(std::string str);
  long long getLine();
  void setLine(long long val);
  long long getCol();
  void setCol(long long val);
};


#endif
