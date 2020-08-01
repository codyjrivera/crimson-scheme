#include <iostream>
#include <list>
#include <string>
#include <vector>

#include "Data.hpp"
#include "Env.hpp"
#include "Error.hpp"
#include "Exp.hpp"
#include "HeapObject.hpp"
#include "Interpreter.hpp"
#include "Lexer.hpp"
#include "Pair.hpp"
#include "Token.hpp"

// Evaluation routine
Data Interpreter::eval(Exp* exp, Env& env) {
    // To handle tail calls
    Data result;
    Exp* evalExp = exp;
    Env* evalEnv = &env;
    bool evalFlag = true;
    while (evalExp != NULL && evalFlag) {
        evalFlag = false;

        // Evaluation dispatch
        if (evalExp->isData()) {
            // Primitive Types and Variable Lookup
            if (evalExp->getData().type == DataType::SYMBOL) {
                result = evalEnv->lookup(evalExp->getData().text);
            } else {
                result = evalExp->getData();
            }
        } else {
            // Language Primitives
            bool primEval = false;
            if (evalExp->getLeft() != NULL && evalExp->getLeft()->isData() &&
                evalExp->getLeft()->getData().type == DataType::SYMBOL) {
                std::string symbol = evalExp->getLeft()->getData().text;
                if (symbol == "define") {
                    result = evalDefine(evalExp, *evalEnv);
                    primEval = true;
                } else if (symbol == "set!") {
                    result = evalSet(evalExp, *evalEnv);
                    primEval = true;
                } else if (symbol == "if") {
                    evalExp = evalIf(evalExp, *evalEnv);
                    primEval = true;
                    evalFlag = true;
                } else if (symbol == "while") {
                    result = evalWhile(evalExp, *evalEnv);
                    primEval = true;
                } else if (symbol == "begin") {
                    // Eval-begin is generic procedure for list of exps
                    evalExp = evalBegin(evalExp->getRight(), *evalEnv);
                    primEval = true;
                    evalFlag = true;
                } else if (symbol == "quote") {
                    result = evalQuote(evalExp->getRight(), *evalEnv);
                    primEval = true;
                }
            }

            if (!primEval) {
                // Procedure Evaluation
                Data proc = eval(evalExp->getLeft(), *evalEnv);
                if (proc.type == DataType::PRIM_PROCEDURE) {
                    // Primitive Procedure Arguments
                    std::vector<Data> args;
                    Exp* argReader = evalExp->getRight();
                    while (argReader != NULL && !argReader->isData()) {
                        args.push_back(eval(argReader->getLeft(), *evalEnv));
                        argReader = argReader->getRight();
                    }
                    // Calls primitive procedure
                    proc.primProcedureVal(result, args, *this);
                } else {
                    throw InterpreterError(
                        "Leftmost expression not a procedure",
                        evalExp->getLine(), evalExp->getCol());
                }
            }
        }
    }
    return result;
}

Data Interpreter::evalDefine(Exp* exp, Env& env) {
    bool status = false;
    std::string id;
    Data value;
    Exp* temp;
    if (exp != NULL) {
        if (exp->getRight() != NULL) {
            temp = exp->getRight();
            if (!temp->isData() && temp->getLeft() != NULL) {
                // Define identifier
                if (temp->getLeft()->isData() &&
                    temp->getLeft()->getData().type == DataType::SYMBOL) {
                    // Extract ID
                    id = temp->getLeft()->getData().text;
                    // Extract value
                    temp = temp->getRight();
                    if (temp != NULL && !temp->isData()) {
                        value = eval(temp->getLeft(), env);
                        // Places identifier in environment
                        env.insert(id, value);
                        status = true;
                    }
                } else {
                    // Define syntactic sugar
                }
            }
        }
    }
    if (status) {
        return value;
    } else {
        throw InterpreterError("Malformed Define Expression", exp->getLine(),
                               exp->getCol());
    }
}

Data Interpreter::evalSet(Exp* exp, Env& env) {
    bool status = false;
    std::string id;
    Data value;
    Exp* temp;
    if (exp != NULL) {
        if (exp->getRight() != NULL) {
            temp = exp->getRight();
            if (!temp->isData() && temp->getLeft() != NULL) {
                // Set! only works on identifiers
                if (temp->getLeft()->isData() &&
                    temp->getLeft()->getData().type == DataType::SYMBOL) {
                    // Extract ID
                    id = temp->getLeft()->getData().text;
                    // Extract value
                    temp = temp->getRight();
                    if (temp != NULL && !temp->isData()) {
                        value = eval(temp->getLeft(), env);
                        // Modifies identifier value
                        env.modify(id, value);
                        status = true;
                    }
                } else {
                    throw InterpreterError("set! only works on identifiers",
                                           exp->getLine(), exp->getCol());
                }
            }
        }
    }
    if (status) {
        return value;
    } else {
        throw InterpreterError("Malformed set! Expression", exp->getLine(),
                               exp->getCol());
    }
}

Exp* Interpreter::evalIf(Exp* exp, Env& env) {
    bool status = false;
    Data value;
    Exp* cont;
    Exp* temp;
    if (exp != NULL) {
        temp = exp->getRight();
        if (temp != NULL && !temp->isData()) {
            value = eval(temp->getLeft(), env);
            temp = temp->getRight();
            if (temp != NULL && !temp->isData()) {
                status = true;
                if (value.type == DataType::BOOLEAN &&
                    value.booleanVal == false) {
                    // Gives alternative
                    if (temp->getRight() != NULL) {
                        cont = temp->getRight()->getLeft();
                    } else {
                        cont = NULL;
                    }
                } else {
                    cont = temp->getLeft();
                }
            }
        }
    }
    if (status) {
        return cont;
    } else {
        throw InterpreterError("Malformed if Expression", exp->getLine(),
                               exp->getCol());
    }
}

Data Interpreter::evalWhile(Exp* exp, Env& env) {
    bool status = false;
    Exp *condition, *body, *cont;
    Data condValue, value;
    Exp* temp;
    if (exp != NULL) {
        temp = exp->getRight();
        if (temp != NULL && !temp->isData()) {
            condition = temp->getLeft();
            body = temp->getRight();

            status = true;
            condValue = eval(condition, env);
            while (!(condValue.type == DataType::BOOLEAN &&
                     condValue.booleanVal == false)) {
                // Evals first statements
                cont = evalBegin(body, env);
                // Evals rest
                value = eval(cont, env);
                condValue = eval(condition, env);
            }
        }
    }
    if (status) {
        return value;
    } else {
        throw InterpreterError("Malformed while Expression", exp->getLine(),
                               exp->getCol());
    }
}

Exp* Interpreter::evalBegin(Exp* exp, Env& env) {
    Exp* cont = NULL;
    Exp* temp = exp;
    while (temp != NULL && !temp->isData()) {
        if (cont != NULL) {
            eval(cont, env);
        }
        cont = temp->getLeft();
        temp = temp->getRight();
    }
    return cont;
}

Data Interpreter::evalQuote(Exp* exp, Env& env) {
    // Expression is null or edge case to deal with empty list (parsed as exp: null null) properly.
    if (exp == NULL ||
        (!exp->isData() && exp->getLeft() == NULL && exp->getRight() == NULL)) {
        return Data::Nil();
    } else if (exp->isData()) {
        return exp->getData();
    } else {
        Pair* newPair = heap.allocNewPair(evalQuote(exp->getLeft(), env),
                                          evalQuote(exp->getRight(), env));
        return Data::Pair(newPair);
    }
}
