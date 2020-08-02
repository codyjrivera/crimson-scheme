#ifndef _Procedure_HPP
#define _Procedure_HPP

#include <exception>
#include <iostream>
#include <string>
#include <vector>

#include "Env.hpp"
#include "Error.hpp"
#include "Exp.hpp"
#include "HeapObject.hpp"

class Procedure : public HeapObject {
   private:
    Env* parent;
    std::vector<std::string> formalParms;
    Exp* body;

   public:
    Procedure();
    Procedure(Env& p, std::vector<std::string>& parms, Exp* b);

    // Needed to deallocate the procedure body
    ~Procedure();

    Env& getParent();
    void setParent(Env& p);

    const std::vector<std::string>& getParms();
    void setParms(std::vector<std::string>& parms);

    Exp* getBody();
    void setBody(Exp* b);

    std::string toString() const override;

    void mark() override;
};

#endif
