#include "Heap.hpp"

#include <list>
#include <vector>

#include "Data.hpp"
#include "Env.hpp"
#include "Exp.hpp"
#include "Pair.hpp"
#include "Procedure.hpp"

Pair* Heap::allocNewPair(Data first, Data rest) {
    gcCheck();
    Pair* p = new Pair(first, rest);
    pairList.push_back(p);
    return p;
}

Procedure* Heap::allocNewProcedure(Env& p, std::vector<std::string>& parms, Exp* b) {
    gcCheck();
    Procedure* proc = new Procedure(p, parms, b);
    procedureList.push_back(proc);
    return proc;
}

Env* Heap::allocNewEnv(Env& p) {
    gcCheck();
    Env* env = new Env(p);
    envList.push_back(env);
    return env;
}

HeapObject& Heap::getRoot() { return *root; }

void Heap::setRoot(HeapObject& r) { root = &r; }

int Heap::getThreshold() { return threshold; }

void Heap::setThreshold(int t) { threshold = t; }

void Heap::runGc() {
    using namespace std;
    reclaimed = 0;
    root->mark();
    list<Pair*>::iterator pairIter = pairList.begin();
    while (pairIter != pairList.end()) {
        if ((*pairIter)->isMarked()) {
            delete *pairIter;
            pairIter = pairList.erase(pairIter);
        } else {
            ++pairIter;
        }
    }

    list<Procedure*>::iterator procedureIter = procedureList.begin();
    while (procedureIter != procedureList.end()) {
        if ((*procedureIter)->isMarked()) {
            delete *procedureIter;
            procedureIter = procedureList.erase(procedureIter);
        } else {
            ++procedureIter;
        }
    }

    list<Env*>::iterator envIter = envList.begin();
    while (envIter != envList.end()) {
        if ((*envIter)->isMarked()) {
            delete *envIter;
            envIter = envList.erase(envIter);
        } else {
            ++envIter;
        }
    }
}

// RAII guarantee
Heap::~Heap() {
    for (Pair* p : pairList) {
        delete p;
    }
    for (Procedure* p : procedureList) {
        delete p;
    }
    for (Env* e : envList) {
        delete e;
    }
}

void Heap::gcCheck() {
    if (count >= static_cast<size_t>(threshold)) {
        runGc();
        count = 0;
    }
    ++count;
} 