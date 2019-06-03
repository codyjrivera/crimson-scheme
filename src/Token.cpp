/* Implementation of functions in class Token */

#include "Token.hpp"
#include <string>
#include <sstream>

// Constructors and destructors
Token::Token() {}

Token::Token(TokenType type, std::string val, long l, long c)
    : tokenType(type), value(val), line(l), col(c)
{
}

Token::~Token() {}

// Copy and Assignment Constructors
Token::Token(const Token& tok)
    : tokenType(tok.tokenType), value(tok.value), line(tok.line), col(tok.col)
{
}

Token& Token::operator=(const Token& tok)
{
    if (&tok != this)
    {
        tokenType = tok.tokenType;
        value = tok.value;
        line = tok.line;
        col = tok.col;
    }
    return *this;
}

// Getters and setters
TokenType Token::getType() const { return tokenType; }

void Token::setType(TokenType type) { tokenType = type; }

std::string Token::getValue() const{ return value; }

void Token::setValue(std::string str) { value = str; }

long Token::getLine() const { return line; }

void Token::setLine(long val) { line = val; }

long Token::getCol() const { return col; }

void Token::setCol(long val) { col = val; }

std::string Token::toString() const
{
    std::string typeString;
    std::stringstream fullStringStream;
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
    fullStringStream << "{ " << typeString << ": " << value << " ("
                     << line << ", " << col << ") }" << std::endl;
    return fullStringStream.str();
}


std::ostream& operator<<(std::ostream& stream, const Token& tok)
{
    stream << tok.toString();
    return stream;
}
