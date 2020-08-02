#include "Procedure.hpp"

#include <exception>
#include <iostream>
#include <string>
#include <vector>

#include "Env.hpp"
#include "Error.hpp"
#include "Exp.hpp"

Procedure::Procedure() : parent(NULL), body(NULL) {}

Procedure::Procedure(Env& p, std::vector<std::string>& parms, Exp* b)
    : parent(&p), formalParms(parms), body(b) {
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

Env& Procedure::getParent() { return *parent; }
void Procedure::setParent(Env& p) { parent = &p; }

const std::vector<std::string>& Procedure::getParms() { return formalParms; }
void Procedure::setParms(std::vector<std::string>& parms) {
    formalParms = parms;
}

Exp* Procedure::getBody() { return body; }
void Procedure::setBody(Exp* b) { body = b; }

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

void Procedure::mark() {
    if (!parent->isMarked()) {
        parent->mark();
    }
}
