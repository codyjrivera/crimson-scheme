#ifndef Pair_HPP
#define Pair_HPP

#include <set>
#include <string>

#include "Data.hpp"
#include "HeapObject.hpp"

class Pair : public HeapObject {
   private:
    Data first, rest;
    std::string toStringNoParens(std::set<const HeapObject*>& visited) const;
    std::string toStringHelper(std::set<const HeapObject*>& visited) const;

   public:
    Pair(Data f, Data r) : first(f), rest(r) {}
    Data getFirst() const { return first; }
    void setFirst(Data f) { first = f; }
    Data getRest() const { return rest; }
    void setRest(Data r) { rest = r; }

    std::string toString() const override;

    void mark() override;
};

#endif
