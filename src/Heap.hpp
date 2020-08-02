#ifndef Heap_HPP
#define Heap_HPP

#include <string>
#include <vector>

#include "Data.hpp"
#include "Env.hpp"
#include "Exp.hpp"
#include "Pair.hpp"
#include "Procedure.hpp"

class Heap {
   private:
    std::vector<Pair*> pairList;
    std::vector<Procedure*> procedureList;
    std::vector<Env*> envList;

   public:
    Pair* allocNewPair(Data first, Data rest);
    Procedure* allocNewProcedure(Env& p, std::vector<std::string>& parms,
                                 Exp* b);
    Env* allocNewEnv(Env& p);
    ~Heap();
};

#endif