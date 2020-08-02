#include "Pair.hpp"

#include <set>
#include <string>

#include "Data.hpp"

std::string Pair::toStringNoParens(std::set<const HeapObject*>& visited) const {
    using namespace std;
    string pre = "";
    switch (first.type) {
        case DataType::PAIR:
            if (visited.find(first.object) == visited.end()) {
                // New object
                visited.insert(first.object);
                pre =
                    " " +
                    static_cast<Pair*>(first.object)->toStringHelper(visited);
            } else {
                pre = "CIRC-REF";
            }
            break;
        default:
            pre = first.toString();
            break;
    }
    string post = "";
    switch (rest.type) {
        // Nil in rest terminates printing
        case DataType::NIL:
            break;
        // List in rest appends list to current list.
        case DataType::PAIR:
            if (visited.find(rest.object) == visited.end()) {
                // New object
                visited.insert(rest.object);
                post =
                    " " +
                    static_cast<Pair*>(rest.object)->toStringNoParens(visited);
            } else {
                post = " CIRC-REF";
            }
            break;
        // Default
        default:
            post = " . " + rest.toString();
            break;
    }
    return pre + post;
}

std::string Pair::toStringHelper(std::set<const HeapObject*>& visited) const {
    return "(" + toStringNoParens(visited) + ")";
}

std::string Pair::toString() const {
    std::set<const HeapObject*> visited;
    visited.insert(this);
    return toStringHelper(visited);
}
