#ifndef _Procedure_HPP
#define _Procedure_HPP

#include <exception>
#include <iostream>
#include <string>
#include <vector>

#include "Env.hpp"
#include "Error.hpp"
#include "Exp.hpp"

class Procedure {
   private:
    Env* parent;
    std::vector<std::string> formalParms;
    Exp* body;

    bool marked;

   public:
    Procedure();
    Procedure(Env& p, std::vector<std::string>& parms, Exp* b);

    // Needed to deallocate the procedure body
    ~Procedure();

    void mark();
    bool isMarked() const;
    void setMark(bool b);

    const Env& getParent() const;
    void setParent(Env& p);

    const std::vector<std::string>& getParms();
    void setParms(std::vector<std::string>& parms);

    Exp* getBody();
    void setBody(Exp* b);
};

#endif
