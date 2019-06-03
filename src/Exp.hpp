#ifndef _Exp_HPP
#define _Exp_HPP

#include <iostream>
#include <string>
#include <exception>
#include "Lexer.hpp"
#include "Data.hpp"
#include "Error.hpp"


class Exp
{
private:
    // For Procedure structures to handle
    // mark - indicates whether this expression is in the loaded program code
    // proc - indicates whether this expression marks a procedure body
    bool mark, proc;
    // Flags
    bool dataFlag;
    
    // Data
    Exp* leftExp;
    Exp* rightExp;
    Data data;

    // Location
    long row, col;
    
public:
    Exp(long r = 1, long c = 0)
        : mark(true), proc(false), dataFlag(false),
          leftExp(NULL), rightExp(NULL), row(r), col(c)
    {
    }
    Exp(const Data& d, long r = 1, long c = 0)
        : mark(true), proc(false), dataFlag(true),
          leftExp(NULL), rightExp(NULL), data(d), row(r), col(c)
    {
    }
    Exp(Exp* left, Exp* right, long r = 1, long c = 0)
        : mark(true), proc(false), dataFlag(false),
          leftExp(left), rightExp(right), row(r), col(c)
    {
    }


    void expMark();
    void expUnmark();
    void cleanup();
    
    bool isProc() const;
    void setProc(bool p);
    bool isData() const;
    void setDataFlag(bool d);

    Exp* getLeft();
    void setLeft(Exp* l);
    Exp* getRight();
    void setRight(Exp* r);
    Data& getData();
    void setData(const Data& d);

    long getRow() const;
    void setRow(long r);
    long getCol() const;
    void setCol(long c);
};


#endif
