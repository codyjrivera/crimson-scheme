/* Implementation of the lexer -- parses according to the rules in Lexer.md */

#include "Lexer.hpp"

#include <cctype>
#include <iostream>
#include <string>
#include <vector>

#include "Token.hpp"

/* Helper Functions */
static bool isSpecial(char ch);
static Token lexSpecial(std::istream& stream, long& line, long& col);
static Token lexIdentifier(std::istream& stream, long& line, long& col);
static Token lexNumber(std::istream& stream, long& line, long& col);
static Token lexString(std::istream& stream, long& line, long& col);
static void skipWhiteSpace(std::istream& stream, long& line, long& col);
static void skipComment(std::istream& stream, long& line, long& col);
static void skipMultilineComment(std::istream& stream, long& line, long& col);

// Lexer Class Essential Functions
Lexer::Lexer() : inStream(&std::cin), line(1), col(0), saveToken(false) {}

Lexer::Lexer(std::istream& stream)
    : inStream(&stream), line(1), col(0), saveToken(false) {}

Lexer::~Lexer() {}

Lexer::Lexer(const Lexer& lex)
    : inStream(lex.inStream),
      line(lex.line),
      col(lex.col),
      nextToken(lex.nextToken),
      saveToken(lex.saveToken) {}

Lexer& Lexer::operator=(const Lexer& lex) {
    if (this != &lex) {
        inStream = lex.inStream;
        line = lex.line;
        col = lex.col;
        nextToken = lex.nextToken;
        saveToken = lex.saveToken;
    }
    return *this;
}

void Lexer::setStream(std::istream& stream) {
    inStream = &stream;
    flush();
}

void Lexer::flush() {
    std::string tmp;
    saveToken = false;
    getline(*inStream, tmp);
    line = 1;
    col = 0;
}

long Lexer::getLine() { return line; }

long Lexer::getCol() { return col; }

Token Lexer::peek() {
    if (saveToken) return nextToken;
    nextToken = lexToken();
    saveToken = true;
    return nextToken;
}

Token Lexer::next() {
    if (saveToken) {
        saveToken = false;
        return nextToken;
    }
    return lexToken();
}

// Lexer driver
Token Lexer::lexToken() {
    char ch;
    ch = inStream->peek();
    while (!inStream->eof()) {
        // Examines the token and calls the appropriate function to lex
        // or skip whitespace
        if (ch == ';') {
            skipComment((*inStream), line, col);
        }
        // Handles Booleans, Multiline Comments, and other hashed elements
        else if (ch == '#') {
            ch = inStream->get();
            col++;
            ch = inStream->peek();
            if (ch == '|') {
                skipMultilineComment((*inStream), line, col);
            } else if (ch == 't' || ch == 'f') {
                ch = inStream->get();
                if (ch == 't')
                    return Token(TokenType::BOOLEAN, "#t", line, col);
                else
                    return Token(TokenType::BOOLEAN, "#f", line, col);
            } else {
                return Token(TokenType::HASH, "#", line, col);
            }
        } else if (ch == '(' || ch == ')' || ch == '\'') {
            return lexSpecial((*inStream), line, col);
        } else if (isdigit(ch) || ch == '-' || ch == '+' || ch == '.') {
            return lexNumber((*inStream), line, col);
        } else if (ch == '"') {
            return lexString((*inStream), line, col);
        } else if (isspace(ch)) {
            skipWhiteSpace((*inStream), line, col);
        }
        // This scheme will allow some really absurd tokens
        // (pun intended in hindsight)
        else {
            return lexIdentifier((*inStream), line, col);
        }
        ch = inStream->peek();
    }
    return Token(TokenType::END, "", line, col);
}

// Lexer Auxilliary Code
// Determines the type of single character token and creates one
Token lexSpecial(std::istream& stream, long& line, long& col) {
    Token tok;
    TokenType type;
    std::string tokenString;
    char ch;
    // Read the char
    ch = stream.get();
    col++;
    switch (ch) {
        case '(':
        case ')':
            type = TokenType::PAREN;
            break;
        case '\'':
            type = TokenType::QUOTE;
            break;
        default:
            return Token(TokenType::END, "", line, col);
    }
    tokenString.push_back(ch);
    return Token(type, tokenString, line, col);
}

// Checks to see if there is a special identifier
bool isSpecial(char ch) {
    if (ch == '(' || ch == ')' || ch == '\'' || ch == '#' || ch == ';')
        return true;
    else
        return false;
}

// Lexes the identifier until invalid characters show up
Token lexIdentifier(std::istream& stream, long& line, long& col) {
    TokenType type = TokenType::IDENTIFIER;
    std::string tokenString;
    char ch;
    while (true) {
        ch = stream.peek();
        if (stream.eof() || isspace(ch) || isSpecial(ch)) break;
        ch = stream.get();
        col++;
        tokenString.push_back(ch);
    }
    return Token(type, tokenString, line, col);
}

// Lexes the number, asserts that the first digit is zero, and then scans
// the rest of the number until an invalid character shows up
Token lexNumber(std::istream& stream, long& line, long& col) {
    TokenType type = TokenType::NUMBER;
    std::string tokenString;
    char ch;
    ch = stream.peek();
    if (!isdigit(ch) && ch != '-' && ch != '.' && ch != '+') {
        return Token(TokenType::END, "", line, col);
    }
    ch = stream.get();
    col++;
    tokenString.push_back(ch);
    ch = stream.peek();
    while (!(stream.eof() || isspace(ch) || isSpecial(ch))) {
        ch = stream.get();
        col++;
        tokenString.push_back(ch);
        ch = stream.peek();
    }
    /* Handles + and - primitives */
    if (tokenString.length() == 1) {
        if (tokenString[0] == '-' || tokenString[0] == '+')
            return Token(TokenType::IDENTIFIER, tokenString, line, col);
        else if (tokenString[0] == '.')
            return Token(TokenType::DOT, tokenString, line, col);
    }
    return Token(type, tokenString, line, col);
}

Token lexString(std::istream& stream, long& line, long& col) {
    TokenType type = TokenType::STRING;
    std::string tokenString;
    char ch;
    ch = stream.get();
    if (ch != '"') {
        return Token(TokenType::END, "", line, col);
    }
    ch = stream.peek();
    while (!(stream.eof())) {
        ch = stream.get();
        // Checks for end of quotation
        if (ch == '"') break;
        // Includes Newlines
        if (ch == '\n') {
            line++;
            col = 0;
        } else
            col++;
        // Handles Escape Sequences
        if (ch == '\\') {
            if (stream.peek() == '"' || stream.peek() == '\\') {
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
void skipWhiteSpace(std::istream& stream, long& line, long& col) {
    char ch;
    ch = stream.peek();
    while (isspace(ch) && !stream.eof()) {
        ch = stream.get();
        if (ch == '\n') {
            line++;
            col = 0;
        } else
            col++;
        ch = stream.peek();
    }
}

// Skips single-line comment, up to and including the newline
void skipComment(std::istream& stream, long& line, long& col) {
    char ch;
    ch = stream.peek();
    while (!stream.eof()) {
        ch = stream.get();
        if (ch == '\n') {
            line++;
            col = 0;
            break;
        } else
            col++;
        ch = stream.peek();
    }
}

// Skips multi-line comment until comment end delimiter or EOF
// Support nested multiline comments
void skipMultilineComment(std::istream& stream, long& line, long& col) {
    long levels = 1;
    char ch;
    ch = stream.peek();
    while (!stream.eof()) {
        ch = stream.get();
        if (ch == '\n') {
            line++;
            col = 0;
        } else {
            col++;
        }
        // Encountering nested comments
        if (ch == '#') {
            if (stream.peek() == '|') {
                // Increments the level if there's a match
                ch = stream.get();
                col++;
                levels++;
            }
        } else if (ch == '|') {
            if (stream.peek() == '#') {
                // Decrements the level when there's a match
                ch = stream.get();
                col++;
                levels--;
                // Breaks if level is 0 again
                if (levels == 0) {
                    break;
                }
            }
        }
        ch = stream.peek();
    }
}
