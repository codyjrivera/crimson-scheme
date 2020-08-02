#ifndef Heap_HPP
#define Heap_HPP

#include <string>
#include <list>

#include "Data.hpp"
#include "Env.hpp"
#include "Exp.hpp"
#include "Pair.hpp"
#include "Procedure.hpp"

class Heap {
   private:
    std::list<Pair*> pairList;
    std::list<Procedure*> procedureList;
    std::list<Env*> envList;

    HeapObject* root;
    int threshold;

    size_t count = 0;
    size_t reclaimed = 0;

    void gcCheck();

   public:
    Pair* allocNewPair(Data first, Data rest);
    Procedure* allocNewProcedure(Env& p, std::vector<std::string>& parms,
                                 Exp* b);
    Env* allocNewEnv(Env& p);

    HeapObject& getRoot();
    void setRoot(HeapObject& root);
    int getThreshold();
    void setThreshold(int t);

    void runGc();

    ~Heap();
};

#endif