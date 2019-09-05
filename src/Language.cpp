#include <iostream>
#include <string>
#include <list>
#include <vector>
#include "Exp.hpp"
#include "Env.hpp"
#include "Data.hpp"
#include "Lexer.hpp"
#include "Token.hpp"
#include "Error.hpp"
#include "Interpreter.hpp"


/* Language Primitive Procedures
   
   Phase 1:
   +, -, *, /, div, mod, truncate
   <, >, =, <=, >=,
   display, newline
   min, max

   Phase 2:
   cons, car, cdr, set-car!, set-cdr!
   read

 */
