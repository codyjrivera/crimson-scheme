#include "Pair.hpp"

#include <string>

#include "Data.hpp"

std::string Pair::toStringNoParens() const {
    using namespace std;
    string pre = first.toString() + " ";
    string post = "";
    switch (rest.type) {
        // Nil in rest terminates printing
        case DataType::NIL:
            break;
        // List in rest appends list to current list.
        case DataType::PAIR:
            post = dynamic_cast<Pair*>(rest.object)->toStringNoParens();
            break;
        // Default 
        default:
            post = ". " + rest.toString();
            break;
    }
    return pre + post;
}

std::string Pair::toString() const {
    return "(" + toStringNoParens() + ")";
}
