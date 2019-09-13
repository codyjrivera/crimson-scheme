# crimson-scheme -- A tree-walk Scheme interpreter written in C++.
By Cody Rivera

This is a simple tree-walk intepreter that implements the Scheme programming language.

So far, the basic special forms of Scheme have been implemented: **begin**, **if**,
**define** (variables only), and **set!**, along with a loop construct **while**.

Go to *src* and type *"make"* to build, and *./cri-scm* to run a REPL.

To run a file, type *./cri-scm* *filename*.


## Sample Programs

Note - to run multiline sample programs in the REPL, wrap (begin ...) around the program.

Example 1. Swapping Variables
```scheme
; variable definitions
(define x 1)
(define y 2)
; variable swap
(define temp x)
(set! x y)
(set! y temp)
```


Example 2. Euclid's Algorithm
```scheme
; Constant Definitions
(define A 111)
(define B 259)

; Program Body 
(define a A)
(define b B)

; Euclid's Algorithm
(while (not (= a b))
       (if (< a b)
           (set! b (- b a))
           (set! a (- a b))))

; a = b = GCD(A, B)
(display (div (+ a b) 2))
(newline)
```


