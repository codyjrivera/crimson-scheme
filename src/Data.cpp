#include "Data.hpp"

#include <exception>
#include <iostream>
#include <string>
#include <vector>

#include "Error.hpp"
#include "Exp.hpp"
#include "Lexer.hpp"

/*
  Data Implementation

  The data-types in this program are implemented as a tagged union, wherever
  possible, of all the types considered.

  Instead of holding advanced structures in itself, Data merely points to
  more advanced structures
 */

bool Data::isPrimitive() const { return primitive; }

std::string Data::toString() const {
    switch (type) {
        case DataType::BOOLEAN:
            if (booleanVal == false) {
                return "#f";
            } else {
                return "#t";
            }
            break;
        case DataType::FLOAT:
            return std::to_string(floatVal);
            break;
        case DataType::INTEGER:
            return std::to_string(integerVal);
            break;
        case DataType::SYMBOL:
            return text;
            break;
        case DataType::STRING:
            return std::string("\"") + text + std::string("\"");
            break;
        case DataType::PRIM_PROCEDURE:
            return std::string("<primitive-procedure ") + text +
                   std::string(">");
            break;
        case DataType::NIL:
            return std::string("()");
            break;
        default:
            return "Unspecified";
    }
}
