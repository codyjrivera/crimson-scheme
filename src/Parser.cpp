#include <iostream>
#include <string>
#include <memory>
#include <exception>
#include "Lexer.hpp"
#include "Token.hpp"
#include "Error.hpp"
#include "Exp.hpp"
#include "Data.hpp"


namespace
{
  // Data parsing helper functions
  std::unique_ptr<Exp> parseBoolean(Lexer& lexer);
  std::unique_ptr<Exp> parseNumber(Lexer& lexer);
  std::unique_ptr<Exp> parseString(Lexer& lexer);
  std::unique_ptr<Exp> parseSymbol(Lexer& lexer);
}


std::unique_ptr<Exp> Exp::parse(Lexer& lexer)
{
  Token tok = lexer.peek();;
  switch (tok.getType())
  {
  case TokenType::END:
    return std::unique_ptr<Exp>(new NoneExp());
  default:
    return parseData(lexer);
  }
}


std::unique_ptr<Exp> Exp::parseData(Lexer& lexer)
{
  Token tok = lexer.peek();
  switch (tok.getType())
  {
  case TokenType::BOOLEAN:
    return parseBoolean(lexer);
  case TokenType::NUMBER:
    return parseNumber(lexer);
  case TokenType::STRING:
    return parseString(lexer);
  case TokenType::IDENTIFIER:
    return parseSymbol(lexer);
  default:
    return std::unique_ptr<Exp>(new NoneExp());
  }
}

namespace
{
  std::unique_ptr<Exp> parseBoolean(Lexer& lexer)
  {
    Token tok = lexer.next();
    if (tok.getValue() == "#t")
    {
      return std::unique_ptr<Exp>(new Boolean(true));
    }
    else
    {
      return std::unique_ptr<Exp>(new Boolean(false));
    }
  }

  std::unique_ptr<Exp> parseNumber(Lexer& lexer)
  {
    Token tok = lexer.next();
    std::string value = tok.getValue();
    try
    {
      // If there are no decimals or 'e's, parse as an integer
      if (value.find('.') == std::string::npos &&
          value.find('e') == std::string::npos)
        return std::unique_ptr<Exp>(new Integer(std::stoll(value)));
      else
        return std::unique_ptr<Exp>(new Real(std::stod(value)));
    }
    catch (std::invalid_argument& e)
    {
      throw new ParseError("Cannot convert to a number: " + tok.getValue(), tok.getLine(), tok.getCol());
    }
    catch (std::out_of_range& e)
    {
      throw new ParseError("Number out of range: " + tok.getValue(), tok.getLine(), tok.getCol());
    }
    return std::unique_ptr<Exp>(new NoneExp());
  }

  std::unique_ptr<Exp> parseString(Lexer& lexer)
  {
    Token tok = lexer.next();
    return std::unique_ptr<Exp>(new String(tok.getValue()));
  }

  std::unique_ptr<Exp> parseSymbol(Lexer& lexer)
  {
    Token tok = lexer.next();
    return std::unique_ptr<Exp>(new Symbol(tok.getValue()));
  }
}
