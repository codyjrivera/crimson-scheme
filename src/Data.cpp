#include <iostream>
#include <string>
#include <vector>
#include <exception>
#include "Lexer.hpp"
#include "Error.hpp"
#include "Exp.hpp"
#include "Data.hpp"

/*
  Data Implementation

  The data-types in this program are implemented as a tagged union, wherever
  possible, of all the types considered. Inheritance and visitors are not
  used, as the visitor pattern is inelegant for primitive data.
  
  Instead of holding advanced structures in itself, Data merely points to
  more advanced structures
 */
