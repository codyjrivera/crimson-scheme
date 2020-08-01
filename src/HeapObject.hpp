#ifndef HeapObject_HPP
#define HeapObject_HPP

#include <string>

class HeapObject {
   public:
    virtual ~HeapObject() {}
    virtual std::string toString() const = 0;
};

#endif