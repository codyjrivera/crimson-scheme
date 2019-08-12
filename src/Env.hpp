#ifndef _Env_HPP
#define _Env_HPP

#include <iostream>
#include <string>
#include <memory>
#include <vector>
#include <unordered_map>
#include "Exp.hpp"
#include "Data.hpp"
#include "Error.hpp"

class Env
{
private:
    Env* top;
    Env* parent;

    std::unordered_map<std::string, Data> table;
    
    bool marked;
    
public:
    Env();
    Env(Env& p);

    void mark();
    bool isMarked() const;
    void setMark(bool b);

    Data lookup(std::string identifier) const;
    void insert(std::string identifier, Data value);
    void modify(std::string identifier, Data value);
};

#endif
