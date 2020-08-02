#include "Heap.hpp"

#include <vector>

#include "Data.hpp"
#include "Env.hpp"
#include "Exp.hpp"
#include "Pair.hpp"
#include "Procedure.hpp"

Pair* Heap::allocNewPair(Data first, Data rest) {
    Pair* p = new Pair(first, rest);
    pairList.push_back(p);
    return p;
}

Procedure* Heap::allocNewProcedure(Env& p, std::vector<std::string>& parms, Exp* b) {
    Procedure* proc = new Procedure(p, parms, b);
    procedureList.push_back(proc);
    return proc;
}

Env* Heap::allocNewEnv(Env& p) {
    Env* env = new Env(p);
    envList.push_back(env);
    return env;
}

// RAII guarantee, even though GC not implemented.
Heap::~Heap() {
    for (Pair* p : pairList) {
        delete p;
    }
}