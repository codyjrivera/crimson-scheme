#ifndef _Env_HPP
#define _Env_HPP

#include <iostream>
#include <memory>
#include <string>
#include <unordered_map>
#include <vector>

#include "Data.hpp"
#include "Error.hpp"
#include "Exp.hpp"
#include "HeapObject.hpp"

class Env : public HeapObject {
   private:
    Env* top;
    Env* parent;

    std::unordered_map<std::string, Data> table;

   public:
    Env();
    Env(Env& p);

    Data lookup(std::string identifier) const;
    void insert(std::string identifier, Data value);
    void modify(std::string identifier, Data value);

    std::string toString() const override;

    void mark() override;
};

#endif
