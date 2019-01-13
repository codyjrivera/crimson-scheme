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
}
