#include <iostream>
#include <string>
#include <sstream>
#include <exception>
#include "Lexer.hpp"
#include "Error.hpp"
#include "Exp.hpp"
#include "Parser.hpp"


/*
  Expression Implementation - Implemented as a 'cons cell' style data structure, with
  left and right descendants.
  
  Carries two mark variables to indicate the expressions usage either in the AST
  or in a procedure object.

  Also implements the parser and beautifier
 */


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




Exp* Parser::parseFile(Lexer& lex)
{
    try
    {
        // Adds begin to beginning of AST
        Exp* newExp = new Exp((new Exp(Data::Symbol(std::string("begin")))), (Exp*) NULL);
        allocNodes.push_back(newExp);
        Exp* currentExp = newExp;
        Exp* parsedExp = parseExp(lex);
        while (parsedExp != NULL)
        {
            // Adds to list and progresses iterator
            currentExp->setRight(new Exp(parsedExp, (Exp*) NULL, parsedExp->getLine(), parsedExp->getCol()));
            allocNodes.push_back(currentExp->getRight());
            currentExp = currentExp->getRight();
            parsedExp = parseExp(lex);
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
    catch (InterpreterError& e)
    {
        for (Exp* exp : allocNodes)
        {
            delete exp;
        }
        throw e;
    }
}




Exp* Parser::parseExp(Lexer& lex)
{
    try
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
    catch (InterpreterError& e)
    {
        for (Exp* exp : allocNodes)
        {
            delete exp;
        }
        throw e;
    }
}


Exp* Parser::parsePrimitive(Lexer& lex)
{
    Token tok = lex.next();
    Exp* newExp;
       
    switch (tok.getType())
    {
    case TokenType::BOOLEAN:
        // To mirror Scheme's rule of truthiness, a false token is the only token
        // that evaluates to false
        if (tok.getValue() == "#f")
        {
            newExp = new Exp(Data::Boolean(false), tok.getLine(), tok.getCol());
            allocNodes.push_back(newExp);
            return newExp;
        }
        else
        {
            newExp = new Exp(Data::Boolean(true), tok.getLine(), tok.getCol());
            allocNodes.push_back(newExp);
            return newExp;
        }
        break;
    case TokenType::STRING:
        newExp = new Exp(Data::String(tok.getValue()));
        allocNodes.push_back(newExp);
        return newExp;
        break;
    case TokenType::IDENTIFIER:
        newExp = new Exp(Data::Symbol(tok.getValue()));
        allocNodes.push_back(newExp);
        return newExp;
        break;
    default:
        throw InterpreterError("Corrupt Token: ' " + tok.getValue() + " '", tok.getLine(), tok.getCol());
        break;
    }
}


Exp* Parser::parseNumber(Lexer& lex)
{
    Token tok = lex.next();
    Exp* newExp;
    std::stringstream strNumStream(tok.getValue());

    long intValue;
    double floatValue;

    if (tok.getValue().find('.') == std::string::npos && strNumStream >> intValue)
    {
        newExp = new Exp(Data::Integer(intValue), tok.getLine(), tok.getCol());
        allocNodes.push_back(newExp);
        return newExp;
    }
    else
    {
        strNumStream.clear();
        strNumStream.str(tok.getValue());
        if (strNumStream >> floatValue)
        {
            newExp = new Exp(Data::Float(floatValue), tok.getLine(), tok.getCol());
            allocNodes.push_back(newExp);
            return newExp;
        }
        else
        {
            throw InterpreterError("Cannot Parse Number ' " + tok.getValue() + " '",
                                   tok.getLine(), tok.getCol());
        }
    }
}



Exp* Parser::parseQuote(Lexer& lex)
{
    // Token stored purely for line and col numbers
    Token tok = lex.next();
    Exp* newExp = new Exp((new Exp(Data::Symbol("quote"), tok.getLine(), tok.getCol())),
                          (Exp*) NULL, tok.getLine(), tok.getCol());
    allocNodes.push_back(newExp);
    newExp->setRight(parseExp(lex));
    return newExp;
}


Exp* Parser::parseHash(Lexer& lex)
{
    // Token stored purely for line and col numbers
    Token tok = lex.next();
    Exp* newExp = new Exp((new Exp(Data::Symbol("hash"), tok.getLine(), tok.getCol())),
                          (Exp*) NULL, tok.getLine(), tok.getCol());
    allocNodes.push_back(newExp);
    newExp->setRight(parseExp(lex));
    return newExp;
}


Exp* Parser::parseSExp(Lexer& lex)
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


Exp* Parser::parseCompoundExp(Lexer& lex)
{
    // No matter what, this will return a pair, as () is a valid s-expression
    Token tok = lex.peek();
    Exp* newExp = new Exp((Exp*) NULL, (Exp*) NULL, tok.getLine(), tok.getCol());
    allocNodes.push_back(newExp);
    newExp->setLeft(parseExp(lex));
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


Exp* Parser::parseCompoundRest(Lexer& lex)
{
    // This potentially will return NULL
    // Iterative implementation of recursive rule
    // Dummy node in stack
    Exp dummyExp((Exp*) NULL, (Exp*) NULL);
    Token tok;
    Exp* currentExp = &dummyExp;
    Exp* parsedExp = parseExp(lex);
    while (parsedExp != NULL)
    {
        currentExp->setRight(new Exp(parsedExp, (Exp*) NULL, parsedExp->getLine(), parsedExp->getCol()));
        allocNodes.push_back(currentExp->getRight());
        currentExp = currentExp->getRight();
        parsedExp = parseExp(lex);
    }
    tok = lex.peek();
    if (tok.getType() == TokenType::DOT)
    {
        // Advances Token
        lex.next();
        parsedExp = parseExp(lex);
        if (parsedExp != NULL)
        {
            currentExp->setRight(new Exp(parsedExp, parsedExp->getLine(), parsedExp->getCol()));
            allocNodes.push_back(currentExp->getRight());
        }
        else
        {
            throw InterpreterError("Expression required after dot", tok.getLine(), tok.getCol());
        }
    }
    return dummyExp.getRight();
}






