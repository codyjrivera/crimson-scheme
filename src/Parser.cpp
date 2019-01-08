#include <iostream>
#include <string>
#include <memory>
#include <exception>
#include "Lexer.hpp"
#include "Token.hpp"
#include "Error.hpp"
#include "Exp.hpp"

std::unique_ptr<Exp> Exp::parse(Lexer& lexer)
{
  Token tok = lexer.peek();
  std::unique_ptr<Exp> data;
  switch (tok.getType())
  {
  case TokenType::END:
    return std::unique_ptr<Exp>(new NoneExp());
  default:
    data = std::move(parseData(lexer));
    tok = lexer.next();
    return data;
  }
}


std::unique_ptr<Exp> Exp::parseData(Lexer& lexer)
{
  Token tok = lexer.peek();
  switch (tok.getType())
  {
  default:
    break;
  }
  return std::unique_ptr<Exp>(new NoneExp());
}
