#ifndef _Error_HPP
#define _Error_HPP

#include <exception>
#include <iostream>
#include <string>

class InterpreterError : public std::runtime_error {
   private:
    long long line, col;

   public:
    InterpreterError(const std::string& error);
    InterpreterError(const std::string& error, long long l, long long c);
    ~InterpreterError();
    long long getLine();
    void setLine(long long l);
    long long getCol();
    void setCol(long long c);
    std::string toString();
};

std::ostream& operator<<(std::ostream& stream, InterpreterError& error);

#endif
