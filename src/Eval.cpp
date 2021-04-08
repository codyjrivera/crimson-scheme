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
                } else if (symbol == "lambda") {
                    result = evalLambda(evalExp, *evalEnv);
                    primEval = true;
                } else if (symbol == "and") {
                    // If not short-circuited
                    if (!evalAnd(evalExp->getRight(), *evalEnv, evalExp, result)) {
                        evalFlag = true;
                    }
                    primEval = true;
                } else if (symbol == "or") {
                    // If not short-circuited
                    if (!evalOr(evalExp->getRight(), *evalEnv, evalExp, result)) {
                        evalFlag = true;
                    }
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
                } else if (proc.type == DataType::PROCEDURE) {
                    Procedure* closure = static_cast<Procedure*>(proc.object);

                    // Read args
                    std::vector<Data> args;
                    Exp* argReader = evalExp->getRight();
                    while (argReader != NULL && !argReader->isData()) {
                        args.push_back(eval(argReader->getLeft(), *evalEnv));
                        argReader = argReader->getRight();
                    }

                    // Match args
                    if (args.size() < closure->getParms().size()) {
                        throw InterpreterError(
                            "Mismatching Arity: Called " +
                            std::to_string(closure->getParms().size()) +
                            "-ary procedure with " +
                            std::to_string(args.size()) + " args");
                    }

                    // Variadic arguments
                    Data varArgsList = Data::Nil();
                    if (args.size() > closure->getParms().size()) {
                        if (closure->isVariadic()) {
                            std::vector<Data> varArgs;
                            size_t i = closure->getParms().size();
                            for (; i < args.size(); ++i) {
                                varArgs.push_back(args[i]);
                            }
                            varArgsList = toSchemeList(varArgs);
                        } else {
                            throw InterpreterError(
                                "Oversupplied nonvariadic procedure: Called " +
                                std::to_string(closure->getParms().size()) +
                                "-ary procedure with " +
                                std::to_string(args.size()) + " args");
                        }
                    }

                    // New Environment
                    Env* newEnv = heap.allocNewEnv(closure->getParent());
                    for (size_t i = 0; i < closure->getParms().size(); ++i) {
                        newEnv->insert(closure->getParms()[i], args[i]);
                    }
                    if (closure->isVariadic()) {
                        newEnv->insert(closure->getVariadicParm(), varArgsList);
                    }

                    // Eval body in new env
                    evalExp = evalBegin(closure->getBody(), *newEnv);
                    evalFlag = true;

                    // Replace old env with new env for tail call
                    evalEnv = newEnv;
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
                    // Try to evaluate as a procedure
                    if (temp->getLeft() != NULL && !temp->getLeft()->isData() &&
                        temp->getLeft()->getLeft() != NULL &&
                        temp->getLeft()->getLeft()->getData().type ==
                            DataType::SYMBOL) {
                        value = evalProcedure(temp->getLeft()->getRight(),
                                              temp->getRight(), env);
                        env.insert(temp->getLeft()->getLeft()->getData().text,
                                   value);
                        status = true;
                    }
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
    // Expression is null or edge case to deal with empty list (parsed as exp:
    // null null) properly.
    Data value;
    if (exp == NULL ||
        (!exp->isData() && exp->getLeft() == NULL && exp->getRight() == NULL)) {
        value = Data::Nil();
    } else if (exp->isData()) {
        value = exp->getData();
    } else {
        Pair* newPair = heap.allocNewPair(evalQuote(exp->getLeft(), env),
                                          evalQuote(exp->getRight(), env));
        value = Data::Pair(newPair);
    }
    return value;
}

Data Interpreter::evalLambda(Exp* exp, Env& env) {
    bool status = false;
    std::string id;
    Data value;
    Exp* temp;
    if (exp != NULL) {
        if (exp->getRight() != NULL) {
            temp = exp->getRight();
            if (temp->getLeft() != NULL && temp->getRight() != NULL) {
                value = evalProcedure(temp->getLeft(), temp->getRight(), env);
                status = true;
            }
        }
    }
    if (status) {
        return value;
    } else {
        throw InterpreterError("Malformed Lambda Expression", exp->getLine(),
                               exp->getCol());
    }
}

// Returns true if the expression short-circuits, placing the output in result
// Otherwise returns false, placing the last subexpression in evalExp
bool Interpreter::evalAnd(Exp* exp, Env& env, Exp*& evalExp, Data& result) {
    Exp* cont = NULL;
    Exp* temp = exp;
    while (temp != NULL && !temp->isData()) {
        if (cont != NULL) {
            Data d = eval(cont, env);
            if (d.type == DataType::BOOLEAN && d.booleanVal == false) {
                result = d;
                return true;
            }
        }
        cont = temp->getLeft();
        temp = temp->getRight();
    }
    evalExp = cont;
    return false;
}

// Returns true if the expression short-circuits, placing the output in result
// Otherwise returns false, placing the last subexpression in evalExp
bool Interpreter::evalOr(Exp* exp, Env& env, Exp*& evalExp, Data& result) {
    Exp* cont = NULL;
    Exp* temp = exp;
    while (temp != NULL && !temp->isData()) {
        if (cont != NULL) {
            Data d = eval(cont, env);
            if (d.type != DataType::BOOLEAN || d.booleanVal != false) {
                result = d;
                return true;
            }
        }
        cont = temp->getLeft();
        temp = temp->getRight();
    }
    evalExp = cont;
    return false;
}

Data Interpreter::evalProcedure(Exp* args, Exp* body, Env& env) {
    Data value;
    std::vector<std::string> paramNames;
    Exp* argReader = args;
    bool variadic = false;
    std::string variadicName;
    while (argReader != NULL && !argReader->isData()) {
        // Special case -- Empty list in (lambda ())
        if (argReader->getLeft() == NULL && argReader->getRight() == NULL) {
            break;
        }
        if (argReader->getLeft()->isData() &&
            argReader->getLeft()->getData().type == DataType::SYMBOL) {
            paramNames.push_back(argReader->getLeft()->getData().text);
        } else {
            throw InterpreterError("Malformed argument list",
                                   argReader->getLine(), argReader->getCol());
        }
        argReader = argReader->getRight();
    }
    if (argReader != NULL && argReader->isData()) {
        if (argReader->getData().type == DataType::SYMBOL) {
            variadic = true;
            variadicName = argReader->getData().text;
        } else {
            throw InterpreterError("Malformed variadic argument",
                                   argReader->getLine(), argReader->getCol());
        }
    }
    Procedure* proc = heap.allocNewProcedure(env, paramNames, body);
    if (variadic) {
        proc->makeVariadic(variadicName);
    }
    value = Data::Procedure(proc);
    return value;
}

Data Interpreter::toSchemeList(std::vector<Data> list) {
    using namespace std;
    Data result = Data::Nil();
    vector<Data>::reverse_iterator iter = list.rbegin();
    while (iter != list.rend()) {
        result = Data::Pair(heap.allocNewPair(*iter, result));
        ++iter;
    }
    return result;
}
