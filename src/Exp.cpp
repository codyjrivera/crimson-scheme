#include <iostream>
#include <string>
#include <exception>
#include "Lexer.hpp"
#include "Error.hpp"
#include "Exp.hpp"


/*
  Expression Implementation - Implemented as a 'cons cell' style data structure, with
  left and right descendants.
  
  Carries two mark variables to indicate the expressions usage either in the AST
  or in a procedure object.
 */


// Tree traversals for mark, unmark, and cleanup
void Exp::expMark()
{
    mark = true;
    if (!dataFlag)
    {
        if (leftExp != NULL)
        {
            leftExp->expMark();
        }
        if (rightExp != NULL)
        {
            rightExp->expMark();
        }
    }
}


void Exp::expUnmark()
{
    mark = false;
    if (!dataFlag)
    {
        if (leftExp != NULL)
        {
            leftExp->expUnmark();
        }
        if (rightExp != NULL)
        {
            rightExp->expUnmark();
        }
    }
}


void Exp::cleanup()
{
    expUnmark();
    if (!proc)
    {
        if (!dataFlag)
        {
            if (leftExp != NULL)
            {
                leftExp->cleanup();
            }
            if (rightExp != NULL)
            {
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

Data& getData() { return data; }
void setData(const Data& d) { data = d; }

long getRow() const { return row; }
void setRow(long r) { row = r; }

long getCol() const { return col; }
void setCol(long c) { col = c; }
