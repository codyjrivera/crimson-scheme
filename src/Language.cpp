#include <iostream>
#include <list>
#include <string>
#include <vector>

#include "Data.hpp"
#include "Env.hpp"
#include "Error.hpp"
#include "Exp.hpp"
#include "Interpreter.hpp"
#include "Lexer.hpp"
#include "Pair.hpp"
#include "Parser.hpp"
#include "Token.hpp"

/* Language Primitive Procedures

   Phase 1:
   +, -, *, /, div, mod, truncate,
   and, or, not,
   <, >, =, <=, >=,
   display, newline
   min, max

   Phase 2:
   cons, car, cdr, set-car!, set-cdr!,
   error, read, eof-object?

 */

// Helper functions
long trueMod(long a, long b);

void primAdd(Data& result, std::vector<Data>& args, Interpreter& interpreter) {
    (void)interpreter;
    long integerSum = 0;
    double floatSum = 0;

    bool promoted = false;
    unsigned long i;

    for (i = 0; i < args.size() && !promoted; ++i) {
        if (args[i].type == DataType::INTEGER) {
            integerSum += args[i].integerVal;
        } else {
            promoted = true;
        }
    }

    if (promoted) {
        floatSum = integerSum;
        for (--i; i < args.size(); ++i) {
            if (args[i].type == DataType::INTEGER) {
                floatSum += args[i].integerVal;
            } else if (args[i].type == DataType::FLOAT) {
                floatSum += args[i].floatVal;
            } else {
                throw InterpreterError(
                    "Primitive Procedure +: Cannot add non-numerical values");
            }
        }
        result = Data::Float(floatSum);
    } else {
        result = Data::Integer(integerSum);
    }
}

void primSub(Data& result, std::vector<Data>& args, Interpreter& interpreter) {
    (void)interpreter;
    long integerSum = 0;
    double floatSum = 0;

    bool promoted = false;
    unsigned long i;

    for (i = 0; i < args.size() && !promoted; ++i) {
        if (args[i].type == DataType::INTEGER) {
            integerSum -= args[i].integerVal;
        } else {
            promoted = true;
        }
        if (i == 0) integerSum = -integerSum;
    }

    if (promoted) {
        floatSum = integerSum;
        for (--i; i < args.size(); ++i) {
            if (args[i].type == DataType::INTEGER) {
                floatSum -= args[i].integerVal;
            } else if (args[i].type == DataType::FLOAT) {
                floatSum -= args[i].floatVal;
            } else {
                throw InterpreterError(
                    "Primitive Procedure -: Cannot subtract non-numerical "
                    "values");
            }
            if (i == 0) integerSum = -integerSum;
        }
        result = Data::Float(floatSum);
    } else {
        result = Data::Integer(integerSum);
    }
}

void primMul(Data& result, std::vector<Data>& args, Interpreter& interpreter) {
    (void)interpreter;
    long integerProd = 1;
    double floatProd = 1;

    bool promoted = false;
    unsigned long i;

    for (i = 0; i < args.size() && !promoted; ++i) {
        if (args[i].type == DataType::INTEGER) {
            integerProd *= args[i].integerVal;
        } else {
            promoted = true;
        }
    }

    if (promoted) {
        floatProd = integerProd;
        for (--i; i < args.size(); ++i) {
            if (args[i].type == DataType::INTEGER) {
                floatProd *= args[i].integerVal;
            } else if (args[i].type == DataType::FLOAT) {
                floatProd *= args[i].floatVal;
            } else {
                throw InterpreterError(
                    "Primitive Procedure *: Cannot multiply non-numerical "
                    "values");
            }
        }
        result = Data::Float(floatProd);
    } else {
        result = Data::Integer(integerProd);
    }
}

void primFloatDiv(Data& result, std::vector<Data>& args,
                  Interpreter& interpreter) {
    (void)interpreter;

    double a, b;

    if (args.size() != 2) {
        throw InterpreterError(
            "Primitive Procedure /: Must be called with 2 arguments");
    }

    if (args[0].type == DataType::INTEGER) {
        a = args[0].integerVal;
    } else if (args[0].type == DataType::FLOAT) {
        a = args[0].floatVal;
    } else {
        throw InterpreterError(
            "Primitive Procedure /: Cannot divide non-numeric values");
    }

    if (args[1].type == DataType::INTEGER) {
        b = args[1].integerVal;
    } else if (args[1].type == DataType::FLOAT) {
        b = args[1].floatVal;
    } else {
        throw InterpreterError(
            "Primitive Procedure /: Cannot divide non-numeric values");
    }

    if (b != 0) {
        result = Data::Float(a / b);
    } else {
        throw InterpreterError("Primitive Procedure /: Cannot divide by zero");
    }
}

void primDiv(Data& result, std::vector<Data>& args, Interpreter& interpreter) {
    (void)interpreter;

    long a, b;

    if (args.size() != 2) {
        throw InterpreterError(
            "Primitive Procedure div: Must be called with 2 arguments");
    }

    if (args[0].type == DataType::INTEGER) {
        a = args[0].integerVal;
    } else {
        throw InterpreterError(
            "Primitive Procedure div: Cannot divide non-integer values, use / "
            "instead");
    }

    if (args[1].type == DataType::INTEGER) {
        b = args[1].integerVal;
    } else {
        throw InterpreterError(
            "Primitive Procedure div: Cannot divide non-integer values, use / "
            "instead");
    }

    if (b != 0) {
        result = Data::Integer(a / b);
    } else {
        throw InterpreterError(
            "Primitive Procedure div: Cannot divide by zero");
    }
}

void primMod(Data& result, std::vector<Data>& args, Interpreter& interpreter) {
    (void)interpreter;

    long a, b;

    if (args.size() != 2) {
        throw InterpreterError(
            "Primitive Procedure mod: Must be called with 2 arguments");
    }

    if (args[0].type == DataType::INTEGER) {
        a = args[0].integerVal;
    } else {
        throw InterpreterError(
            "Primitive Procedure mod: Cannot take the modulus of non-integer "
            "values");
    }

    if (args[1].type == DataType::INTEGER) {
        b = args[1].integerVal;
    } else {
        throw InterpreterError(
            "Primitive Procedure mod: Cannot take the modulus of non-integer "
            "values");
    }

    if (b != 0) {
        // Calculates true modulus
        result = Data::Integer(trueMod(a, b));
    } else {
        throw InterpreterError(
            "Primitive Procedure mod: Cannot take a number mod zero");
    }
}

void primNot(Data& result, std::vector<Data>& args, Interpreter& interpreter) {
    (void)interpreter;

    bool isTrue = true;

    if (args.size() != 1) {
        throw InterpreterError(
            "Primitive Procedure not: Requires one argument");
    } else {
        if (args[0].type == DataType::BOOLEAN && args[0].booleanVal == false) {
            isTrue = false;
        } else {
            isTrue = true;
        }
    }

    result = Data::Boolean(!isTrue);
}

void primLessThan(Data& result, std::vector<Data>& args,
                  Interpreter& interpreter) {
    (void)interpreter;

    long a, b;
    double af, bf;

    bool promoted = false;

    if (args.size() != 2) {
        throw InterpreterError(
            "Primitive Procedure <: Must be called with 2 arguments");
    }

    if (args[0].type == DataType::INTEGER) {
        a = args[0].integerVal;
    } else if (args[0].type == DataType::FLOAT) {
        af = args[0].floatVal;
        promoted = true;
    } else {
        throw InterpreterError(
            "Primitive Procedure <: Cannot compare non-numeric values");
    }

    if (args[1].type == DataType::INTEGER) {
        if (promoted) {
            bf = args[1].integerVal;
        } else {
            b = args[1].integerVal;
        }
    } else if (args[1].type == DataType::FLOAT) {
        bf = args[1].floatVal;
        af = a;
        promoted = true;
    } else {
        throw InterpreterError(
            "Primitive Procedure <: Cannot compare non-numeric values");
    }

    if (promoted) {
        result = Data::Boolean(af < bf);
    } else {
        result = Data::Boolean(a < b);
    }
}

void primGreaterThan(Data& result, std::vector<Data>& args,
                     Interpreter& interpreter) {
    (void)interpreter;

    long a, b;
    double af, bf;

    bool promoted = false;

    if (args.size() != 2) {
        throw InterpreterError(
            "Primitive Procedure >: Must be called with 2 arguments");
    }

    if (args[0].type == DataType::INTEGER) {
        a = args[0].integerVal;
    } else if (args[0].type == DataType::FLOAT) {
        af = args[0].floatVal;
        promoted = true;
    } else {
        throw InterpreterError(
            "Primitive Procedure >: Cannot compare non-numeric values");
    }

    if (args[1].type == DataType::INTEGER) {
        if (promoted) {
            bf = args[1].integerVal;
        } else {
            b = args[1].integerVal;
        }
    } else if (args[1].type == DataType::FLOAT) {
        bf = args[1].floatVal;
        af = a;
        promoted = true;
    } else {
        throw InterpreterError(
            "Primitive Procedure >: Cannot compare non-numeric values");
    }

    if (promoted) {
        result = Data::Boolean(af > bf);
    } else {
        result = Data::Boolean(a > b);
    }
}

void primLessThanEqual(Data& result, std::vector<Data>& args,
                       Interpreter& interpreter) {
    (void)interpreter;

    long a, b;
    double af, bf;

    bool promoted = false;

    if (args.size() != 2) {
        throw InterpreterError(
            "Primitive Procedure <=: Must be called with 2 arguments");
    }

    if (args[0].type == DataType::INTEGER) {
        a = args[0].integerVal;
    } else if (args[0].type == DataType::FLOAT) {
        af = args[0].floatVal;
        promoted = true;
    } else {
        throw InterpreterError(
            "Primitive Procedure <=: Cannot compare non-numeric values");
    }

    if (args[1].type == DataType::INTEGER) {
        if (promoted) {
            bf = args[1].integerVal;
        } else {
            b = args[1].integerVal;
        }
    } else if (args[1].type == DataType::FLOAT) {
        bf = args[1].floatVal;
        af = a;
        promoted = true;
    } else {
        throw InterpreterError(
            "Primitive Procedure <=: Cannot compare non-numeric values");
    }

    if (promoted) {
        result = Data::Boolean(af <= bf);
    } else {
        result = Data::Boolean(a <= b);
    }
}

void primGreaterThanEqual(Data& result, std::vector<Data>& args,
                          Interpreter& interpreter) {
    (void)interpreter;

    long a, b;
    double af, bf;

    bool promoted = false;

    if (args.size() != 2) {
        throw InterpreterError(
            "Primitive Procedure >=: Must be called with 2 arguments");
    }

    if (args[0].type == DataType::INTEGER) {
        a = args[0].integerVal;
    } else if (args[0].type == DataType::FLOAT) {
        af = args[0].floatVal;
        promoted = true;
    } else {
        throw InterpreterError(
            "Primitive Procedure >=: Cannot compare non-numeric values");
    }

    if (args[1].type == DataType::INTEGER) {
        if (promoted) {
            bf = args[1].integerVal;
        } else {
            b = args[1].integerVal;
        }
    } else if (args[1].type == DataType::FLOAT) {
        bf = args[1].floatVal;
        af = a;
        promoted = true;
    } else {
        throw InterpreterError(
            "Primitive Procedure >=: Cannot compare non-numeric values");
    }

    if (promoted) {
        result = Data::Boolean(af >= bf);
    } else {
        result = Data::Boolean(a >= b);
    }
}

void primEqual(Data& result, std::vector<Data>& args,
               Interpreter& interpreter) {
    (void)interpreter;

    long a, b;
    double af, bf;

    bool promoted = false;

    if (args.size() != 2) {
        throw InterpreterError(
            "Primitive Procedure =: Must be called with 2 arguments");
    }

    if (args[0].type == DataType::INTEGER) {
        a = args[0].integerVal;
    } else if (args[0].type == DataType::FLOAT) {
        af = args[0].floatVal;
        promoted = true;
    } else {
        throw InterpreterError(
            "Primitive Procedure =: Cannot compare non-numeric values");
    }

    if (args[1].type == DataType::INTEGER) {
        if (promoted) {
            bf = args[1].integerVal;
        } else {
            b = args[1].integerVal;
        }
    } else if (args[1].type == DataType::FLOAT) {
        bf = args[1].floatVal;
        af = a;
        promoted = true;
    } else {
        throw InterpreterError(
            "Primitive Procedure =: Cannot compare non-numeric values");
    }

    if (promoted) {
        result = Data::Boolean(af == bf);
    } else {
        result = Data::Boolean(a == b);
    }
}

void primDisplay(Data& result, std::vector<Data>& args,
                 Interpreter& interpreter) {
    if (args.size() == 1) {
        if (args[0].type == DataType::STRING) {
            *(interpreter.output) << args[0].text;
        } else {
            *(interpreter.output) << interpreter.dataToString(args[0]);
        }
        result = args[0];
    } else if (args.size() == 2) {
        throw InterpreterError(
            "Primitive Procedure display: Ports not implemented");
    } else {
        throw InterpreterError("Primitive Procedure display: maximum arity 2");
    }
}

void primNewline(Data& result, std::vector<Data>& args,
                 Interpreter& interpreter) {
    (void)result;
    if (args.size() == 0) {
        *(interpreter.output) << std::endl;
    } else {
        throw InterpreterError("Primitive Procedure newline: has no arguments");
    }
}

void primCons(Data& result, std::vector<Data>& args, Interpreter& interpreter) {
    if (args.size() == 2) {
        Pair* newPair = interpreter.heap.allocNewPair(args[0], args[1]);
        result = Data::Pair(newPair);
    } else {
        throw InterpreterError(
            "Primitive Procedure cons: must be called with two arguments");
    }
}

void primCar(Data& result, std::vector<Data>& args, Interpreter& interpreter) {
    (void)interpreter;

    if (args.size() == 1) {
        if (args[0].type == DataType::PAIR) {
            result = static_cast<Pair*>(args[0].object)->getFirst();
        } else {
            throw InterpreterError(
                "Primitive Procedure car: cannot take car of non-pair value");
        }
    } else {
        throw InterpreterError(
            "Primitive Procedure car: must be called with one argument");
    }
}

void primCdr(Data& result, std::vector<Data>& args, Interpreter& interpreter) {
    (void)interpreter;

    if (args.size() == 1) {
        if (args[0].type == DataType::PAIR) {
            result = static_cast<Pair*>(args[0].object)->getRest();
        } else {
            throw InterpreterError(
                "Primitive Procedure cdr: cannot take cdr of non-pair value");
        }
    } else {
        throw InterpreterError(
            "Primitive Procedure cdr: must be called with one argument");
    }
}

void primSetCar(Data& result, std::vector<Data>& args,
                Interpreter& interpreter) {
    (void)interpreter;
    (void)result;

    if (args.size() == 2) {
        if (args[0].type == DataType::PAIR) {
            static_cast<Pair*>(args[0].object)->setFirst(args[1]);
        } else {
            throw InterpreterError(
                "Primitive Procedure set-car!: first argument must be a pair");
        }
    } else {
        throw InterpreterError(
            "Primitive Procedure set-car!: must be called with two arguments");
    }
}

void primSetCdr(Data& result, std::vector<Data>& args,
                Interpreter& interpreter) {
    (void)interpreter;
    (void)result;

    if (args.size() == 2) {
        if (args[0].type == DataType::PAIR) {
            static_cast<Pair*>(args[0].object)->setRest(args[1]);
        } else {
            throw InterpreterError(
                "Primitive Procedure set-cdr!: first argument must be a pair");
        }
    } else {
        throw InterpreterError(
            "Primitive Procedure set-cdr!: must be called with two arguments");
    }
}

void primError(Data& result, std::vector<Data>& args,
               Interpreter& interpreter) {
    (void) result;
    (void) interpreter;

    std::string message = "Runtime Error:";
    for (Data arg : args) {
        message += "\n";
        if (arg.type == DataType::STRING) {
            message += arg.text;
        } else {
            message += arg.toString();
        }
    }
    throw InterpreterError(message);
}

// To implement read, basically parse an exp and evalQuote it.
void primRead(Data& result, std::vector<Data>& args,
                 Interpreter& interpreter) {
    if (args.size() == 0) {
        Parser parser;
        Lexer lexer(*(interpreter.input));
        Exp* exp, quoteExp;
        try {
            exp = parser.parseExp(lexer);
            result = interpreter.evalQuote(exp, interpreter.topEnv);
        } catch (InterpreterError& e) {
            if (exp != NULL) {
                exp->cleanup();
                delete exp;
            }
            if (lexer.peek().getType() == TokenType::END) {
                result = Data::EndOfFile();
                return;
            } else {
                // rethrow
                throw e;
            }
        }
        if (exp != NULL) {
            exp->cleanup();
            delete exp;
        }
    } else if (args.size() == 1) {
        throw InterpreterError(
            "Primitive Procedure read: Ports not implemented");
    } else {
        throw InterpreterError("Primitive Procedure read: maximum arity 1");
    }
}

void primEOFObject(Data& result, std::vector<Data>& args, Interpreter& interpreter) {
    (void)interpreter;

    if (args.size() != 1) {
        throw InterpreterError(
            "Primitive Procedure eof-object?: Requires one argument");
    } else {
        result = args[0].type == DataType::END_OF_FILE;
    }
}

void Interpreter::initInterpreter() {
    // Deposits primitive procedures
    topEnv.insert("+", Data::PrimProcedure("+", primAdd));
    topEnv.insert("-", Data::PrimProcedure("-", primSub));
    topEnv.insert("*", Data::PrimProcedure("*", primMul));
    topEnv.insert("/", Data::PrimProcedure("/", primFloatDiv));
    topEnv.insert("div", Data::PrimProcedure("div", primDiv));
    topEnv.insert("mod", Data::PrimProcedure("mod", primMod));
    topEnv.insert("not", Data::PrimProcedure("not", primNot));
    topEnv.insert(">", Data::PrimProcedure(">", primGreaterThan));
    topEnv.insert("<", Data::PrimProcedure("<", primLessThan));
    topEnv.insert(">=", Data::PrimProcedure(">=", primGreaterThanEqual));
    topEnv.insert("<=", Data::PrimProcedure("<=", primLessThanEqual));
    topEnv.insert("=", Data::PrimProcedure("=", primEqual));
    topEnv.insert("display", Data::PrimProcedure("display", primDisplay));
    topEnv.insert("newline", Data::PrimProcedure("newline", primNewline));
    topEnv.insert("cons", Data::PrimProcedure("cons", primCons));
    topEnv.insert("car", Data::PrimProcedure("car", primCar));
    topEnv.insert("cdr", Data::PrimProcedure("cdr", primCdr));
    topEnv.insert("set-car!", Data::PrimProcedure("set-car!", primSetCar));
    topEnv.insert("set-cdr!", Data::PrimProcedure("set-cdr!", primSetCdr));
    topEnv.insert("error", Data::PrimProcedure("error", primError));
    topEnv.insert("read", Data::PrimProcedure("read", primRead));
    topEnv.insert("eof-object?", Data::PrimProcedure("eof-object?", primEOFObject));

    // Sets up GC
    heap.setRoot(topEnv);
    heap.setThreshold(10000);
}

long trueMod(long a, long b) {
    long mod;
    if (b < 0) b = -b;
    mod = a % b;
    if (mod < 0) mod = mod + b;
    return mod;
}
