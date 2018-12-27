# crimson-scheme -- A recursive Scheme interpreter written in C++.
By Cody Rivera

This interpreter will implement a superset of a subset of R4RS Scheme. 

This interpeter will eventually provide several extensions to
Scheme, as stated below:

## Call-by-Reference:
Any variable beginning with '&' will be a reference variable;
programs can bind other variables to it as well as pass it
to other functions.

Example:  
(define a 1)  
((lambda (&x) (set! x (+ x 1))) a)
a

=> 2


## Lazy evaluation:
Any variable beginning with $ will represent a thunk;
any valid expression can be bound to $, and will only be evaluated
if the variable has to be evaluated.

Example:  
(define a 2)  
(define b (- 2 a))  
(set! a 1)  
((lambda ($x) (/ 1 $x)) b)  

=> 1

