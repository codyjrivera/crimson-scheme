#include <iostream>
#include <string>
#include <memory>
#include <vector>
#include <unordered_map>
#include <stdexcept>
#include "Exp.hpp"
#include "Data.hpp"
#include "Error.hpp"
#include "Env.hpp"

// Simple Environment Structure - Holds Variables and Primitive Values

Env::Env() : marked(false)
{
    top = parent = this;
}

Env::Env(Env& p) : top(p.top), parent(&p), marked(false)
{
    
}


void Env::mark()
{
    marked = true;
}


bool Env::isMarked() const
{
    return marked;
}


void Env::setMark(bool b)
{
    marked = b;
}



Data Env::lookup(std::string identifier) const
{
    try
    {
        return table.at(identifier);
    }
    catch (std::out_of_range& e)
    {
        if (parent != NULL && parent != this)
        {
            return parent->lookup(identifier);
        }
        else
        {
            throw InterpreterError(std::string("Identifier '") + identifier + std::string("' is not defined in this context"));
        }
    }
}



void Env::insert(std::string identifier, Data value)
{
    table.emplace(identifier, value);
}


void Env::modify(std::string identifier, Data value)
{
    try
    {
        table.at(identifier) = value;
    }
    catch (std::out_of_range& e)
    {
        if (parent != NULL && parent != this)
        {
            parent->modify(identifier, value);
        }
        else
        {
            throw InterpreterError(identifier + std::string(" is not defined in this context"));
        }
    }
}
