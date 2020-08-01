#ifndef _Data_HPP
#define _Data_HPP

#include "Error.hpp"
#include "Lexer.hpp"
#include <exception>
#include <iostream>
#include <string>
#include <vector>

class Procedure;
class Env;
class Interpreter;

enum class DataType
{
    // Primitive Data
    BOOLEAN,
    INTEGER,
    FLOAT,
    SYMBOL,
    STRING,
    PRIM_PROCEDURE,
    UNDEFINED,
    NIL,

    // Compound Data
    PAIR,
    PROCEDURE,
    ENVIRONMENT
};

struct Data
{
    DataType type;
    bool primitive;
    std::string text;
    // Union of primitive data types
    union
    {
        bool booleanVal;
        long integerVal;
        double floatVal;
        void (*primProcedureVal)(Data&, std::vector<Data>&, Interpreter&);
        unsigned long pairIndex;
        // Procedure* procVal;
        Env* envVal;
    };

    // Constructors
    Data() : type(DataType::UNDEFINED), primitive(false) {}
    Data(bool b) : type(DataType::BOOLEAN), primitive(true), booleanVal(b) {}
    Data(long i) : type(DataType::INTEGER), primitive(true), integerVal(i) {}
    Data(double f) : type(DataType::FLOAT), primitive(true), floatVal(f) {}
    Data(DataType t, std::string s = std::string(""))
        : type(t), primitive(true), text(s)
    {
    }
    Data(std::string name, void (*p)(Data&, std::vector<Data>&, Interpreter&))
        : type(DataType::PRIM_PROCEDURE), primitive(true), text(name),
          primProcedureVal(p)
    {
    }

    // Named Constructors
    static Data Boolean(bool b) { return Data(b); }
    static Data Integer(long i) { return Data(i); }
    static Data Float(double f) { return Data(f); }
    static Data Symbol(std::string s) { return Data(DataType::SYMBOL, s); }
    static Data String(std::string s) { return Data(DataType::STRING, s); }
    static Data PrimProcedure(std::string name,
                              void (*p)(Data&, std::vector<Data>&,
                                        Interpreter&))
    {
        return Data(name, p);
    }
    static Data Nil() { return Data(DataType::NIL, "()"); }

    bool isPrimitive() const;
    std::string toString() const;
};

#endif
