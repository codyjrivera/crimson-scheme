#include "Procedure.hpp"

#include <exception>
#include <iostream>
#include <string>
#include <vector>

#include "Env.hpp"
#include "Error.hpp"
#include "Exp.hpp"

Procedure::Procedure() : parent(NULL), body(NULL), marked(false) {}

Procedure::Procedure(Env& p, std::vector<std::string>& parms, Exp* b)
    : parent(&p), formalParms(parms), body(b), marked(false) {
    if (body != NULL) {
        body->setProc(true);
    }
}

Procedure::~Procedure() {
    if (body != NULL) {
        body->setProc(false);
        body->cleanup();
    }
}

std::string Procedure::toString() const {
    using namespace std;
    string result = "<compound-procedure ";
    result += "(arity: " + to_string(formalParms.size()) + ") ( ";
    for (string parm : formalParms) {
        result += parm;
        result += " ";
    }
    result += ")>";
    return result;
}
