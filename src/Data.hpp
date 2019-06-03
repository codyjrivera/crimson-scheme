#ifndef _Data_HPP
#define _Data_HPP

#include <iostream>
#include <string>
#include <vector>
#include <exception>
#include "Lexer.hpp"
#include "Error.hpp"
#include "Exp.hpp"

enum class DataType
{
    // Primitive Data
    BOOLEAN,
    INTEGER,
    FLOAT,
    SYMBOL,
    STRING,
    PRIM_PROCEDURE,
    UNDEFINED
    
    // Compound Data to come later
};


struct Data
{
    DataType type;
    std::string text;
    // Union of primitive data types
    union
    {
        bool booleanVal;
        long integerVal;
        double floatVal;
        void (*primProcedureVal)(Data&, std::vector<Data>&);
    };

    // Constructors
    Data() : type(DataType::UNDEFINED) {}
    Data(bool b) : type(DataType::BOOLEAN), booleanVal(b) {}
    Data(long i) : type(DataType::INTEGER), integerVal(i) {}
    Data(double f) : type(DataType::FLOAT), floatVal(f) {}
    Data(DataType t, std::string s = std::string("")) : type(t), text(s) {}
    Data(std::string name, void (*p)(Data&, std::vector<Data>&))
        : type(DataType::PRIM_PROCEDURE), text(name), primProcedureVal(p)
    {
    }

    // Named Constructors
    static Data Boolean(bool b) { return Data(b); }
    static Data Integer(long i) { return Data(i); }
    static Data Float(double f) { return Data(f); }
    static Data Symbol(std::string s) { return Data(DataType::SYMBOL, s); }
    static Data String(std::string s) { return Data(DataType::STRING, s); }
    static Data PrimProcedure(std::string name, void (*p)(Data&, std::vector<Data>&))
    {
        return Data(name, p);
    }
};


#endif
