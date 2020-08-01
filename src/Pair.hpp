#ifndef Pair_HPP
#define Pair_HPP

#include <string>

#include "Data.hpp"
#include "HeapObject.hpp"

class Pair : HeapObject {
   private:
    Data first, rest;
    std::string toStringNoParens() const;

   public:
    Pair(Data f, Data r) : first(f), rest(r) {}
    Data getFirst() const { return first; }
    void setFirst(Data f) { first = f; }
    Data getRest() const { return rest; }
    void setRest(Data r) { rest = r; }

    std::string toString() const override;
};

#endif
