#ifndef _Data_HPP
#define _Data_HPP

#include <exception>
#include <iostream>
#include <string>
#include <vector>

#include "Error.hpp"
#include "HeapObject.hpp"
#include "Lexer.hpp"

class Procedure;
class Env;
class Interpreter;

enum class DataType {
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

struct Data {
    DataType type;
    bool primitive;
    std::string text;
    // Union of primitive data types
    union {
        bool booleanVal;
        long integerVal;
        double floatVal;
        void (*primProcedureVal)(Data&, std::vector<Data>&, Interpreter&);
        unsigned long pairIndex;
        HeapObject* object;
    };

    // Constructors
    Data() : type(DataType::UNDEFINED), primitive(true) {}
    Data(bool b) : type(DataType::BOOLEAN), primitive(true), booleanVal(b) {}
    Data(long i) : type(DataType::INTEGER), primitive(true), integerVal(i) {}
    Data(double f) : type(DataType::FLOAT), primitive(true), floatVal(f) {}
    Data(DataType t, std::string s = std::string(""))
        : type(t), primitive(true), text(s) {}
    Data(std::string name, void (*p)(Data&, std::vector<Data>&, Interpreter&))
        : type(DataType::PRIM_PROCEDURE),
          primitive(true),
          text(name),
          primProcedureVal(p) {}
    Data(DataType t, HeapObject* obj) : type(t), primitive(false), object(obj) {}

    // Named Constructors
    static Data Boolean(bool b) { return Data(b); }
    static Data Integer(long i) { return Data(i); }
    static Data Float(double f) { return Data(f); }
    static Data Symbol(std::string s) { return Data(DataType::SYMBOL, s); }
    static Data String(std::string s) { return Data(DataType::STRING, s); }
    static Data PrimProcedure(std::string name,
                              void (*p)(Data&, std::vector<Data>&,
                                        Interpreter&)) {
        return Data(name, p);
    }
    static Data Nil() { return Data(DataType::NIL, "()"); }
    static Data Pair(HeapObject* pair) { return Data(DataType::PAIR, pair); }

    bool isPrimitive() const;
    std::string toString() const;
};

#endif
