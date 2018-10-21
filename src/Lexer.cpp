/* Implementation of the lexer -- parses according to the rules in Lexer.md */

#include <istream>
#include <vector>
#include <string>
#include <cctype>
#include "Lexer.hpp"
#include "Token.hpp"

// Lexer Class Essential Functions
Lexer::Lexer(){};
Lexer::~Lexer(){};

Lexer::Lexer(const Lexer& lexer) : tokens(lexer.tokens) {}
Lexer& Lexer::operator=(Lexer& lexer)
{
  tokens = lexer.tokens;
  return *this;
}

void Lexer::clear()
{
  tokens.clear();
}

TokenIterator Lexer::getTokens()
{
  return tokens.begin();
}


// Lexer Code

// Auxilliary Lexing Functions

bool isSpecial(char ch);
Token lexSpecial(std::istream& stream, long long& line, long long &col);
Token lexIdentifier(std::istream& stream, long long& line, long long &col);
Token lexNumber(std::istream& stream, long long& line, long long &col);
Token lexString(std::istream& stream, long long& line, long long &col);
void skipWhiteSpace(std::istream&stream, long long& line, long long &col);
void skipComment(std::istream& stream, long long& line, long long &col);
void skipMultilineComment(std::istream& stream, long long& line, long long &col);

void Lexer::lex(std::istream& stream)
{
  Token tok;
  long long line = 1, col = 0;
  char ch;
  while (true)
  {
    ch = stream.peek();
    if (stream.eof())
      break;
    // Examines the token and calls the appropriate function to lex
    // or skip whitespace
    if (ch == ';')
    {
      skipComment(stream, line, col);
      continue;
    }
    // TODO: Support proper booleans
    else if (ch == '#')
    {
      ch = stream.get();
      col++;
      if (stream.peek() == '|')
      {
        skipMultilineComment(stream, line, col);
        continue;
      }
      tok.setType(HASH);
      tok.setToken("#");
      tok.setLine(line);
      tok.setCol(col);
    }
    else if (ch == '(' || ch == ')' || ch == '\'')
    {
      tok = lexSpecial(stream, line, col);
    }
    else if (isdigit(ch))
    {
      tok = lexNumber(stream, line, col);
    }
    else if (ch == '"')
    {
      tok = lexString(stream, line, col);
    }
    else if (isspace(ch))
    {
      skipWhiteSpace(stream, line, col);
    }
    // This scheme will allow some really absurd tokens
    // (pun intended in hindsight)
    else
    {
      tok = lexIdentifier(stream, line, col);
    }
    tokens.push_back(tok);
  }
}


/* Implementation of functions (NOTE: Guaranteed that at least one character
   can be parsed) */

// Determines the type of single character token and creates one
Token lexSpecial(std::istream& stream, long long& line, long long &col)
{
  Token tok;
  TokenType type;
  std::string tokenString;
  char ch;
  // Read the char
  ch = stream.get();
  col++;
  switch (ch)
  {
  case '(':
  case ')':
    type = PAREN;
    break;
  case '\'':
    type = QUOTE;
    break;
  default:
    throw LexerException();
  }
  tokenString.push_back(ch);
  tok.setToken(tokenString);
  tok.setType(type);
  tok.setLine(line);
  tok.setCol(col);
  return tok;
}

// Checks to see if there is a special identifier
bool isSpecial(char ch)
{
  if (ch == '(' || ch == ')' || ch == '\'' ||
      ch == '#' || ch == ';')
    return true;
  else
    return false;
}

// Lexes the identifier until invalid characters show up
Token lexIdentifier(std::istream& stream, long long& line, long long &col)
{
  Token tok;
  TokenType type = IDENTIFIER;
  std::string tokenString;
  char ch;
  while (true)
  {
    ch = stream.peek();
    if (stream.eof() || isspace(ch) || isSpecial(ch))
      break;
    ch = stream.get();
    col++;
    tokenString.push_back(ch);
  }
  tok.setToken(tokenString);
  tok.setType(type);
  tok.setLine(line);
  tok.setCol(col);
  return tok;
}


// Lexes the number, asserts that the first digit is zero, and then scans
// the rest of the number until an invalid character shows up
Token lexNumber(std::istream& stream, long long& line, long long &col)
{
  Token tok;
  TokenType type = NUMBER;
  std::string tokenString;
  char ch;
  ch = stream.peek();
  if (!isdigit(ch))
  {
    throw LexerException();
  }
  while (!(stream.eof() || isspace(ch) || isSpecial(ch)))
  {
    ch = stream.get();
    col++;
    tokenString.push_back(ch);
    ch = stream.peek();
  }
  tok.setToken(tokenString);
  tok.setType(type);
  tok.setLine(line);
  tok.setCol(col);
  return tok;
}


Token lexString(std::istream& stream, long long& line, long long &col)
{
  Token tok;
  TokenType type = STRING;
  std::string tokenString;
  char ch;
  ch = stream.get();
  if (ch != '"')
  {
    throw LexerException();
  }
  ch = stream.peek();
  while (!(stream.eof() || ch == '"'))
  {
    ch = stream.get();
    // Includes Newlines
    if (ch == '\n')
    {
      line++;
      col = 0;
    }
    else
      col++;
    // Handles Escape Sequences
    if (ch == '\\')
    {
      if (stream.peek() == '"' ||
          stream.peek() == '\\')
      {
        ch = stream.get();
        col++;
      }
    }   
    tokenString.push_back(ch);
    ch = stream.peek();
  }
  tok.setToken(tokenString);
  tok.setType(type);
  tok.setLine(line);
  tok.setCol(col);
  return tok;
}

// Skips whitespace until stream points to the first non-whitespace character
void skipWhiteSpace(std::istream& stream, long long& line, long long &col)
{
  char ch;
  ch = stream.peek();
  while (isspace(ch) && !stream.eof())
  {
    ch = stream.get();
    if (ch == '\n')
    {
      line++;
      col = 0;
    }
    else
      col++;
    ch = stream.peek();
  }
}

// Skips single-line comment, up to and including the newline
void skipComment(std::istream& stream, long long& line, long long &col)
{
  char ch;
  ch = stream.peek();
  while (!stream.eof())
  {
    ch = stream.get();
    if (ch == '\n')
    {
      line++;
      col = 0;
      break;
    }
    else
      col++;
    ch = stream.peek();
  }
}

// Skips multi-line comment until comment end delimiter or EOF 
void skipMulilineComment(std::istream& stream, long long& line, long long &col)
{
  char ch;
  ch = stream.peek();
  while (!stream.eof())
  {
    ch = stream.get();
    if (ch == '\n')
    {
      line++;
      col = 0;
      break;
    }
    else
      col++;
    if (ch == '|')
    {
      if (stream.peek() == '#')
      {
        ch = stream.get();
        col++;
        break;
      }
    }
    ch = stream.peek();
  }
}



// Exception handling
LexerException::LexerException() : errorString("Fatal error in input") {}
const char* LexerException::what() {return errorString.c_str();}

