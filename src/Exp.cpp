#include <iostream>
#include <string>
#include <sstream>
#include <exception>
#include "Lexer.hpp"
#include "Error.hpp"
#include "Exp.hpp"


/*
  Expression Implementation - Implemented as a 'cons cell' style data structure, with
  left and right descendants.
  
  Carries two mark variables to indicate the expressions usage either in the AST
  or in a procedure object.

  Also implements the parser and beautifier
 */


// Tree traversals for mark, unmark, and cleanup
void Exp::expMark()
{
    mark = true;
    if (!dataFlag)
    {
        if (leftExp != NULL)
        {
            leftExp->expMark();
        }
        if (rightExp != NULL)
        {
            rightExp->expMark();
        }
    }
}


void Exp::expUnmark()
{
    mark = false;
    if (!dataFlag)
    {
        if (leftExp != NULL)
        {
            leftExp->expUnmark();
        }
        if (rightExp != NULL)
        {
            rightExp->expUnmark();
        }
    }
}


void Exp::cleanup()
{
    expUnmark();
    if (!proc)
    {
        if (!dataFlag)
        {
            if (leftExp != NULL)
            {
                leftExp->cleanup();
            }
            if (rightExp != NULL)
            {
                rightExp->cleanup();
            }
            delete leftExp;
            delete rightExp;
        }   
    }
}

bool Exp::isProc() const { return proc; }
void Exp::setProc(bool p) { proc = p; }

bool Exp::isData() const { return dataFlag; }
void Exp::setDataFlag(bool d) { dataFlag = d; }

Exp* Exp::getLeft() { return leftExp; }
void Exp::setLeft(Exp* l) { leftExp = l; }

Exp* Exp::getRight() { return rightExp; }
void Exp::setRight(Exp* r) { rightExp = r; }

Data& Exp::getData() { return data; }
void Exp::setData(const Data& d) { data = d; }

long Exp::getLine() const { return line; }
void Exp::setLine(long r) { line = r; }

long Exp::getCol() const { return col; }
void Exp::setCol(long c) { col = c; }



/*
  Parser - Parses S-Expressions

  Grammar:
  File  ->  Exp File | Empty (NOTE - (begin E) will be wrapped around parsed files)
  Exp   ->  Boolean | Number | String | Symbol | QExp | HExp | SExp | Empty
  QExp  ->  ' Exp
  HExp  ->  '#' Exp
  SExp  ->  '(' CExp ')'
  CExp  ->  Exp RCExp | Empty
  RCExp ->  Exp RCExp | '.' Exp | Empty
  
 */


static Exp* parsePrimitive(Lexer& lex);
static Exp* parseNumber(Lexer& lex);
static Exp* parseQuote(Lexer& lex);
static Exp* parseHash(Lexer& lex);
static Exp* parseSExp(Lexer& lex);
static Exp* parseCompoundExp(Lexer& lex);
static Exp* parseCompoundRest(Lexer& lex);



Exp* Exp::parseFile(Lexer& lex)
{
    // Adds begin to beginning of AST
    Exp* newExp = new Exp((new Exp(Data::Symbol(std::string("begin")))), (Exp*) NULL);
    Exp* currentExp = newExp;
    Exp* parsedExp = Exp::parseExp(lex);
    while (parsedExp != NULL)
    {
        // Adds to list and progresses iterator
        currentExp->setRight(new Exp(parsedExp, (Exp*) NULL, parsedExp->getLine(), parsedExp->getCol()));
        currentExp = currentExp->getRight();
        parsedExp = Exp::parseExp(lex);
    }
    // Check for extraneous tokens
    Token tok = lex.peek();
    if (tok.getType() != TokenType::END)
    {
        throw InterpreterError("Unexpected Token: ' " + tok.getValue() + " '",
                               tok.getLine(), tok.getCol());
    }
    return newExp;
}




Exp* Exp::parseExp(Lexer& lex)
{
    Token tok = lex.peek();
    switch (tok.getType())
    {
    case TokenType::BOOLEAN:
    case TokenType::STRING:
    case TokenType::IDENTIFIER:
        return parsePrimitive(lex);
        break;
    case TokenType::NUMBER:
        return parseNumber(lex);
        break;
    case TokenType::QUOTE:
        return parseQuote(lex);
        break;
    case TokenType::HASH:
        return parseHash(lex);
        break;
    case TokenType::PAREN:
        // Triggers a level of S-Expression parsing if left paren
        if (tok.getValue() == "(")
        {
            return parseSExp(lex);
        }
        else
        {
            return NULL;
        }
        break;
    default:
        return NULL;
        break;
    }
}


Exp* parsePrimitive(Lexer& lex)
{
    Token tok = lex.next();
    switch (tok.getType())
    {
    case TokenType::BOOLEAN:
        // To mirror Scheme's rule of truthiness, a false token is the only token
        // that evaluates to false
        if (tok.getValue() == "#f")
        {
            return new Exp(Data::Boolean(false), tok.getLine(), tok.getCol());
        }
        else
        {
            return new Exp(Data::Boolean(true), tok.getLine(), tok.getCol());
        }
        break;
    case TokenType::STRING:
        return new Exp(Data::String(tok.getValue()));
        break;
    case TokenType::IDENTIFIER:
        return new Exp(Data::Symbol(tok.getValue()));
        break;
    default:
        throw InterpreterError("Corrupt Token: ' " + tok.getValue() + " '", tok.getLine(), tok.getCol());
        break;
    }
}


Exp* parseNumber(Lexer& lex)
{
    Token tok = lex.next();
    std::stringstream strNumStream(tok.getValue());

    long intValue;
    double floatValue;

    if (strNumStream >> intValue)
    {
        return new Exp(Data::Integer(intValue), tok.getLine(), tok.getCol());
    }
    else
    {
        strNumStream.clear();
        strNumStream.str(tok.getValue());
        if (strNumStream >> floatValue)
        {
            return new Exp(Data::Float(floatValue), tok.getLine(), tok.getCol());
        }
        else
        {
            throw InterpreterError("Cannot Parse Number ' " + tok.getValue() + " '",
                                   tok.getLine(), tok.getCol());
        }
    }
}



Exp* parseQuote(Lexer& lex)
{
    // Token stored purely for line and col numbers
    Token tok = lex.next();
    Exp* newExp = new Exp((new Exp(Data::Symbol("quote"), tok.getLine(), tok.getCol())),
                          (Exp*) NULL, tok.getLine(), tok.getCol());
    newExp->setRight(Exp::parseExp(lex));
    return newExp;
}


Exp* parseHash(Lexer& lex)
{
    // Token stored purely for line and col numbers
    Token tok = lex.next();
    Exp* newExp = new Exp((new Exp(Data::Symbol("hash"), tok.getLine(), tok.getCol())),
                          (Exp*) NULL, tok.getLine(), tok.getCol());
    newExp->setRight(Exp::parseExp(lex));
    return newExp;
}


Exp* parseSExp(Lexer& lex)
{
    // Token purely advanced to next one
    lex.next();
    
    Exp* newExp = parseCompoundExp(lex);

    // Check for close-paren
    Token tok = lex.peek();
    if (tok.getType() == TokenType::PAREN && tok.getValue() == ")")
    {
        tok = lex.next();
        return newExp;
    }
    else
    {
        throw InterpreterError("Expected ' ) ' but got: ' " + tok.getValue() + " '",
                               tok.getLine(), tok.getCol());
    }
}


Exp* parseCompoundExp(Lexer& lex)
{
    // No matter what, this will return a pair, as () is a valid s-expression
    Token tok = lex.peek();
    Exp* newExp = new Exp((Exp*) NULL, (Exp*) NULL, tok.getLine(), tok.getCol());
    newExp->setLeft(Exp::parseExp(lex));
    if (newExp->getLeft() != NULL)
    {
        newExp->setRight(parseCompoundRest(lex));
    }
    else
    {
        tok = lex.peek();
        if (tok.getType() == TokenType::DOT)
        {
            throw InterpreterError("Dot cannot be placed as first element of compound statement",
                                   tok.getLine(), tok.getCol());
        }
    }
    return newExp;
}


Exp* parseCompoundRest(Lexer& lex)
{
    // This potentially will return NULL
    // Iterative implementation of recursive rule
    // Dummy node in stack
    Exp dummyExp((Exp*) NULL, (Exp*) NULL);
    Token tok;
    Exp* currentExp = &dummyExp;
    Exp* parsedExp = Exp::parseExp(lex);
    while (parsedExp != NULL)
    {
        currentExp->setRight(new Exp(parsedExp, (Exp*) NULL, parsedExp->getLine(), parsedExp->getCol()));
        currentExp = currentExp->getRight();
        parsedExp = Exp::parseExp(lex);
    }
    tok = lex.peek();
    if (tok.getType() == TokenType::DOT)
    {
        // Advances Token
        lex.next();
        parsedExp = Exp::parseExp(lex);
        if (parsedExp != NULL)
        {
            currentExp->setRight(new Exp(parsedExp, parsedExp->getLine(), parsedExp->getCol()));
        }
        else
        {
            throw InterpreterError("Expression required after dot", tok.getLine(), tok.getCol());
        }
    }
    return dummyExp.getRight();
}






