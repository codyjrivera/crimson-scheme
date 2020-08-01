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
    : parent(&p), formalParms(parms), body(b), marked(false) {}

Procedure::~Procedure() {
    parent->setMark(false);
    if (body != NULL) {
        body->setProc(false);
        body->cleanup();
    }
}

void Procedure::mark() { marked = true; }
