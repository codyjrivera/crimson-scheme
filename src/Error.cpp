#include "Error.hpp"

#include <exception>
#include <iostream>
#include <sstream>
#include <string>

#include "Exp.hpp"

// Implementation of InterpreterError

InterpreterError::InterpreterError(const std::string& error)
    : std::runtime_error(error), line(0), col(0) {}

InterpreterError::InterpreterError(const std::string& error, long long l,
                                   long long c)
    : std::runtime_error(error), line(l), col(c) {}

InterpreterError::~InterpreterError() {}

long long InterpreterError::getLine() { return line; }

void InterpreterError::setLine(long long l) { line = l; }

long long InterpreterError::getCol() { return col; }

void InterpreterError::setCol(long long c) { col = c; }

std::string InterpreterError::toString() {
    std::stringstream stream;
    stream << "At (line: " << line << ", col: " << col << "): " << std::endl
           << "Error: " << what() << std::endl;
    return stream.str();
}

std::ostream& operator<<(std::ostream& stream, InterpreterError& error) {
    stream << error.toString();
    return stream;
}
