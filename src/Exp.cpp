#include "Exp.hpp"

#include <exception>
#include <iostream>
#include <sstream>
#include <string>

#include "Error.hpp"
#include "Lexer.hpp"

/*
  Expression Implementation - Implemented as a 'cons cell' style data structure,
  with left and right descendants.

  Carries two mark variables to indicate the expressions usage either in the AST
  or in a procedure object.

  Also implements the parser and beautifier
 */

// Tree traversals for mark, unmark, and cleanup
void Exp::expMark() {
    mark = true;
    if (!dataFlag) {
        if (leftExp != NULL) {
            leftExp->expMark();
        }
        if (rightExp != NULL) {
            rightExp->expMark();
        }
    }
}

void Exp::expUnmark() {
    mark = false;
    if (!dataFlag) {
        if (leftExp != NULL) {
            leftExp->expUnmark();
        }
        if (rightExp != NULL) {
            rightExp->expUnmark();
        }
    }
}

void Exp::cleanup() {
    expUnmark();
    if (!proc) {
        if (!dataFlag) {
            if (leftExp != NULL) {
                leftExp->cleanup();
            }
            if (rightExp != NULL) {
                rightExp->cleanup();
            }
            delete leftExp;
            delete rightExp;
        }
    }
}

bool Exp::isProc() const { return proc; }
void Exp::setProc(bool p) { proc = p; }

bool Exp::isData() const { return dataFlag; }
void Exp::setDataFlag(bool d) { dataFlag = d; }

Exp* Exp::getLeft() { return leftExp; }
void Exp::setLeft(Exp* l) { leftExp = l; }

Exp* Exp::getRight() { return rightExp; }
void Exp::setRight(Exp* r) { rightExp = r; }

Data& Exp::getData() { return data; }
void Exp::setData(const Data& d) { data = d; }

long Exp::getLine() const { return line; }
void Exp::setLine(long r) { line = r; }

long Exp::getCol() const { return col; }
void Exp::setCol(long c) { col = c; }
