#ifndef Heap_HPP
#define Heap_HPP

#include <vector>

#include "Data.hpp"
#include "Pair.hpp"

class Heap {
   private:
    std::vector<Pair*> pairList;

   public:
    Pair* allocNewPair(Data first, Data rest);
    ~Heap();
};

#endif