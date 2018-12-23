/* Implementation of the lexer -- parses according to the rules in Lexer.md */

#include <iostream>
#include <vector>
#include <string>
#include <cctype>
#include "Lexer.hpp"
#include "Token.hpp"


/* Helper Functions */
namespace
{
  bool isSpecial(char ch);
  Token lexSpecial(std::istream& stream, long long& line, long long &col);
  Token lexIdentifier(std::istream& stream, long long& line, long long &col);
  Token lexNumber(std::istream& stream, long long& line, long long &col);
  Token lexString(std::istream& stream, long long& line, long long &col);
  void skipWhiteSpace(std::istream&stream, long long& line, long long &col);
  void skipComment(std::istream& stream, long long& line, long long &col);
  void skipMultilineComment(std::istream& stream, long long& line, long long &col);
}



// Lexer Class Essential Functions
Lexer::Lexer()
  : inStream(&std::cin),
    line(1),
    col(0) {}

Lexer::Lexer(std::istream& stream)
  : inStream(&stream),
    line(1),
    col(0) {}

Lexer::~Lexer() {}

Lexer::Lexer(const Lexer& lex)
  : inStream(lex.inStream),
    line(lex.line),
    col(lex.col) {}

Lexer& Lexer::operator=(const Lexer& lex)
{
  inStream = lex.inStream;
  line = lex.line;
  col = lex.col;
  return *this;
}

void Lexer::setStream(std::istream& stream)
{
  inStream = &stream;
}

void Lexer::resetPosition()
{
  line = 1;
  col = 0;
}

long long Lexer::getLine()
{
  return line;
}

long long Lexer::getCol()
{
  return col;
}

// Lexer driver
Token Lexer::nextToken()
{
  char ch;
  ch = (*inStream).peek();
  while (!(*inStream).eof())
  {
    // Examines the token and calls the appropriate function to lex
    // or skip whitespace
    if (ch == ';')
    {
      skipComment((*inStream), line, col);
    }
    // Handles Booleans, Multiline Comments, and other hashed elements
    else if (ch == '#')
    {
      ch = (*inStream).get();
      col++;
      ch = (*inStream).peek();
      if (ch == '|')
      {
        skipMultilineComment((*inStream), line, col);
      }
      else if (ch == 't' || ch == 'f')
      {
        ch = (*inStream).get();
        if (ch == 't')
          return Token(BOOLEAN, "#t", line, col);
        else
          return Token(BOOLEAN, "#f", line, col);
      }
      else
      {
        return Token(HASH, "#", line, col);
      }
    }
    else if (ch == '(' || ch == ')' || ch == '\'')
    {
      return lexSpecial((*inStream), line, col);
    }
    else if (isdigit(ch) || ch == '-')
    {
      return lexNumber((*inStream), line, col);
    }
    else if (ch == '"')
    {
      return lexString((*inStream), line, col);
    }
    else if (isspace(ch))
    {
      skipWhiteSpace((*inStream), line, col);
    }
    // This scheme will allow some really absurd tokens
    // (pun intended in hindsight)
    else
    {
      return lexIdentifier((*inStream), line, col);
    }
    ch = (*inStream).peek();
  }
  return Token(END, "", line, col);
}


// Lexer Auxilliary Code
namespace
{
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
      return Token(END, "", line, col);
    }
    tokenString.push_back(ch);
    return Token(type, tokenString, line, col);
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
    return Token(type, tokenString, line, col);
  }


  // Lexes the number, asserts that the first digit is zero, and then scans
  // the rest of the number until an invalid character shows up
  Token lexNumber(std::istream& stream, long long& line, long long &col)
  {
    TokenType type = NUMBER;
    std::string tokenString;
    char ch;
    ch = stream.peek();
    if (!isdigit(ch) && ch != '-' && ch != '.')
    {
      return Token(END, "", line, col);
    }
    while (!(stream.eof() || isspace(ch) || isSpecial(ch)))
    {
      ch = stream.get();
      col++;
      tokenString.push_back(ch);
      ch = stream.peek();
    }
    return Token(type, tokenString, line, col);
  }


  Token lexString(std::istream& stream, long long& line, long long &col)
  {
    TokenType type = STRING;
    std::string tokenString;
    char ch;
    ch = stream.get();
    if (ch != '"')
    {
      return Token(END, "", line, col);
    }
    ch = stream.peek();
    while (!(stream.eof()))
    {
      ch = stream.get();
      // Checks for end of quotation
      if (ch == '"')
        break;
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
    return Token(type, tokenString, line, col);
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
  void skipMultilineComment(std::istream& stream, long long& line, long long &col)
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
}



