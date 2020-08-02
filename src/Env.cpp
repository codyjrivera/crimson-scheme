#include "Env.hpp"

#include <iostream>
#include <memory>
#include <stdexcept>
#include <string>
#include <unordered_map>
#include <vector>

#include "Data.hpp"
#include "Error.hpp"
#include "Exp.hpp"

// Simple Environment Structure - Holds Variables and Primitive Values

Env::Env() { top = parent = this; }

Env::Env(Env& p) : top(p.top), parent(&p) {}

Data Env::lookup(std::string identifier) const {
    try {
        return table.at(identifier);
    } catch (std::out_of_range& e) {
        if (parent != NULL && parent != this) {
            return parent->lookup(identifier);
        } else {
            throw InterpreterError(
                std::string("Identifier '") + identifier +
                std::string("' is not defined in this context"));
        }
    }
}

void Env::insert(std::string identifier, Data value) {
    table.emplace(identifier, value);
}

void Env::modify(std::string identifier, Data value) {
    try {
        table.at(identifier) = value;
    } catch (std::out_of_range& e) {
        if (parent != NULL && parent != this) {
            parent->modify(identifier, value);
        } else {
            throw InterpreterError(
                identifier + std::string(" is not defined in this context"));
        }
    }
}

std::string Env::toString() const {
    if (parent == this) {
        return "<top-level environment>";
    } else {
        return "<environment>";
    }
}

void Env::mark() {
    markFlag = true;
    for (std::pair<std::string, Data> entry : table) {
        if (!entry.second.isPrimitive()) {
            if (!entry.second.object->isMarked()) {
                entry.second.object->mark();
            }
        }
    }
}
