#include "Interpreter.hpp"
#include "Error.hpp"
#include "Exp.hpp"
#include "Lexer.hpp"
#include "Token.hpp"
#include <iostream>
#include <limits>
#include <string>

// Implementation of top-level interpreter class

void Interpreter::setOutput(std::ostream& stream) { output = &stream; }

void Interpreter::setAux(std::ostream& stream) { aux = &stream; }
