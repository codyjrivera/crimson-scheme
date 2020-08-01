
/*
  Interpreter.cpp -- Implementation of the interpreter class auxiliary functions

  Written by Cody Rivera

  This file is dedicated to implementing printing and maintenance routines for
  Interpreter


 */

#include "Interpreter.hpp"

#include <iostream>
#include <limits>
#include <string>

#include "Error.hpp"
#include "Exp.hpp"
#include "Lexer.hpp"
#include "Parser.hpp"
#include "Token.hpp"

Interpreter::~Interpreter() {
    if (program != NULL) {
        program->cleanup();
        delete program;
    }
}

void Interpreter::readExp(Lexer& lex) {
    Parser p;
    if (program != NULL) {
        program->cleanup();
        delete program;
        program = NULL;
    }
    program = p.parseExp(lex);
}

void Interpreter::readFile(Lexer& lex) {
    Parser p;
    if (program != NULL) {
        program->cleanup();
        delete program;
        program = NULL;
    }
    program = p.parseFile(lex);
}

std::string Interpreter::dataToString(Data& result) {
    return result.toString();
}

std::string Interpreter::programToString() { return prettyPrint(*program); }

void Interpreter::eval() {
    if (program != NULL) {
        result = eval(program, topEnv);
    }
}

std::string Interpreter::resultToString() { return dataToString(result); }

void Interpreter::setOutput(std::ostream& stream) { output = &stream; }

void Interpreter::setAux(std::ostream& stream) { aux = &stream; }

// Pretty Printer
static void prettyPrintExp(Exp* exp, std::string& out, long indent);
static void prettyPrintExpRest(Exp* exp, std::string& out, long indent);
static void prettyPrintBegin(Exp* exp, std::string& out, long indent);
static void prettyPrintDefine(Exp* exp, std::string& out, long indent);
static void prettyPrintIf(Exp* exp, std::string& out, long indent);
static void prettyPrintWhile(Exp* exp, std::string& out, long indent);
static void prettyPrintLambda(Exp* exp, std::string& out, long indent);

std::string Interpreter::prettyPrint(Exp& exp) {
    std::string resultString = "";

    prettyPrintExp(&exp, resultString, 0);

    return resultString;
}

void prettyPrintExp(Exp* exp, std::string& out, long indent) {
    if (exp != NULL) {
        if (exp->isData()) {
            out.append(exp->getData().toString());
        } else {
            Exp* left = exp->getLeft();
            // Checks for special rules
            // applying to syntactic forms
            if (left != NULL) {
                if (left->isData() &&
                    left->getData().type == DataType::SYMBOL) {
                    std::string& keyword = left->getData().text;
                    // Check for syntactical forms
                    if (keyword == "begin") {
                        prettyPrintBegin(exp, out, indent);
                        return;
                    } else if (keyword == "define") {
                        prettyPrintDefine(exp, out, indent);
                        return;
                    } else if (keyword == "if") {
                        prettyPrintIf(exp, out, indent);
                        return;
                    } else if (keyword == "while") {
                        prettyPrintWhile(exp, out, indent);
                        return;
                    } else if (keyword == "lambda") {
                        prettyPrintLambda(exp, out, indent);
                        return;
                    }
                }
            }
            out.append(1, '(');
            if (left == NULL) {
                out.append("()");
            } else {
                prettyPrintExp(left, out, indent);
            }

            Exp* right = exp->getRight();
            if (right != NULL) {
                if (right->isData()) {
                    out.append(" . ");
                    prettyPrintExpRest(right, out, indent);
                    out.append(1, ')');
                } else {
                    out.append(1, ' ');
                    prettyPrintExpRest(right, out, indent);
                    out.append(1, ')');
                }
            } else {
                out.append(1, ')');
            }
        }
    }
}

void prettyPrintExpRest(Exp* exp, std::string& out, long indent) {
    if (exp != NULL) {
        if (exp->isData()) {
            out.append(exp->getData().toString());
        } else {
            Exp* left = exp->getLeft();
            if (left == NULL) {
                out.append("()");
            } else {
                prettyPrintExp(left, out, indent);
            }

            Exp* right = exp->getRight();
            if (right != NULL) {
                if (right->isData()) {
                    out.append(" . ");
                    prettyPrintExpRest(right, out, indent);
                } else {
                    out.append(1, ' ');
                    prettyPrintExpRest(right, out, indent);
                }
            }
        }
    }
}

void prettyPrintBegin(Exp* exp, std::string& out, long indent) {
    const long BEGIN_INDENT = 2;
    long newIndent = indent + BEGIN_INDENT;
    Exp* iter = exp->getRight();
    // Common elements
    out.append("(begin");

    while (iter != NULL && !iter->isData()) {
        out.append(1, '\n');
        out.append(newIndent, ' ');
        if (iter->getLeft() != NULL) {
            prettyPrintExp(iter->getLeft(), out, newIndent);
        } else {
            out.append("()");
        }
        iter = iter->getRight();
    }

    // Edge Case
    if (iter != NULL && iter->isData()) {
        out.append(" . ");
        prettyPrintExp(iter, out, indent);
    }

    out.append(1, ')');
}

void prettyPrintDefine(Exp* exp, std::string& out, long indent) {
    const long DEFINE_INDENT = 2;
    long newIndent = indent + DEFINE_INDENT;
    Exp* iter = exp->getRight();
    // Common elements
    out.append("(define");

    // First Element
    if (iter != NULL && !iter->isData() && !iter->getLeft()->isData()) {
        // Handles function define syntactic sugar
        // Prints function prototype
        out.append(1, ' ');
        prettyPrintExp(iter->getLeft(), out, indent);
        iter = iter->getRight();
        // Prints function body
        while (iter != NULL && !iter->isData()) {
            out.append(1, '\n');
            out.append(newIndent, ' ');
            if (iter->getLeft() != NULL) {
                prettyPrintExp(iter->getLeft(), out, newIndent);
            } else {
                out.append("()");
            }
            iter = iter->getRight();
        }
        // Edge Case
        if (iter != NULL && iter->isData()) {
            out.append(" . ");
            prettyPrintExp(iter, out, indent);
        }
    } else if (iter != NULL && !iter->isData() && iter->getLeft()->isData()) {
        // Handles flat define

        size_t len_before = out.size(), diff;
        out.append(1, ' ');
        prettyPrintExp(iter->getLeft(), out, indent);
        out.append(1, ' ');
        diff = out.size() - len_before;
        iter = iter->getRight();
        prettyPrintExpRest(iter, out, indent + diff + 7);
    }

    out.append(1, ')');
}

void prettyPrintIf(Exp* exp, std::string& out, long indent) {
    const long IF_INDENT = 4;
    long newIndent = indent + IF_INDENT;
    Exp* iter = exp->getRight();
    // Common elements
    out.append("(if");

    if (iter != NULL) {
        out.append(1, ' ');
        prettyPrintExp(iter->getLeft(), out, indent);
        iter = iter->getRight();
        // Prints function body
        while (iter != NULL && !iter->isData()) {
            out.append(1, '\n');
            out.append(newIndent, ' ');
            if (iter->getLeft() != NULL) {
                prettyPrintExp(iter->getLeft(), out, newIndent);
            } else {
                out.append("()");
            }
            iter = iter->getRight();
        }
    }
    // Edge Case
    if (iter != NULL && iter->isData()) {
        out.append(" . ");
        prettyPrintExp(iter, out, indent);
    }
    out.append(1, ')');
}

void prettyPrintWhile(Exp* exp, std::string& out, long indent) {
    const long WHILE_INDENT = 7;
    long newIndent = indent + WHILE_INDENT;
    Exp* iter = exp->getRight();
    // Common elements
    out.append("(while");

    if (iter != NULL) {
        out.append(1, ' ');
        prettyPrintExp(iter->getLeft(), out, indent);
        iter = iter->getRight();
        // Prints function body
        while (iter != NULL && !iter->isData()) {
            out.append(1, '\n');
            out.append(newIndent, ' ');
            if (iter->getLeft() != NULL) {
                prettyPrintExp(iter->getLeft(), out, newIndent);
            } else {
                out.append("()");
            }
            iter = iter->getRight();
        }
    }
    // Edge Case
    if (iter != NULL && iter->isData()) {
        out.append(" . ");
        prettyPrintExp(iter, out, indent);
    }
    out.append(1, ')');
}

void prettyPrintLambda(Exp* exp, std::string& out, long indent) {
    const long LAMBDA_INDENT = 2;
    long newIndent = indent + LAMBDA_INDENT;
    Exp* iter = exp->getRight();
    // Common elements
    out.append("(lambda");

    if (iter != NULL) {
        out.append(1, ' ');
        prettyPrintExp(iter->getLeft(), out, indent);
        iter = iter->getRight();
        // Prints function body
        while (iter != NULL && !iter->isData()) {
            out.append(1, '\n');
            out.append(newIndent, ' ');
            if (iter->getLeft() != NULL) {
                prettyPrintExp(iter->getLeft(), out, newIndent);
            } else {
                out.append("()");
            }
            iter = iter->getRight();
        }
    }
    // Edge Case
    if (iter != NULL && iter->isData()) {
        out.append(" . ");
        prettyPrintExp(iter, out, indent);
    }
    out.append(1, ')');
}
