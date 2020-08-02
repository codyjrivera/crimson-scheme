#ifndef HeapObject_HPP
#define HeapObject_HPP

#include <string>

class HeapObject {
   protected:
    bool markFlag = false;

   public:
    virtual ~HeapObject() {}
    virtual std::string toString() const = 0;

    // GC
    virtual void mark() = 0;
    virtual bool isMarked() const { return markFlag; };
    virtual void unmark() { markFlag = false; }
};

#endif