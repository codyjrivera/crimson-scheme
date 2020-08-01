#include "Heap.hpp"

#include <vector>

#include "Data.hpp"
#include "Pair.hpp"

Pair* Heap::allocNewPair(Data first, Data rest) {
    Pair* p = new Pair(first, rest);
    pairList.push_back(p);
    return p;
}

// RAII guarantee, even though GC not implemented.
Heap::~Heap() {
    for (Pair* p : pairList) {
        delete p;
    }
}