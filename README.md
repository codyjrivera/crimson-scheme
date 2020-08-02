# crimson-scheme -- A tree-walk Scheme interpreter written in C++.
By Cody Rivera

This is a simple tree-walk intepreter that implements the Scheme programming language.

So far, the basic special forms of Scheme have been implemented: **begin**, **if**,
**define**, **lambda**, and **set!**, along with an extended loop construct **while**.

This interpreter also fully implements Scheme's lexical scoping, as well as a restricted
subset of Scheme's primitives.

Go to *src* and type *"make"* to build, and *./cri-scm* to run a REPL.

To run a file, type *./cri-scm* *filename*.


## Sample Programs

Note - to run multiline sample programs in the REPL, wrap (begin ...) around the program.

Example 1. Euclid's Algorithm
```scheme
(define A 111)
(define B 259)

; Euclid's Algorithm
(define (gcd a b)
  (if (= a b)
      a ; or b, this is arbitrary
      (if (< a b)
          (gcd a (- b a))
          (gcd (- a b) b))))

; GCD(A, B) = 37
(display (gcd A B))
(newline)
```

Example 2. Traditional [Man or Boy](https://en.wikipedia.org/wiki/Man_or_boy_test) Test
```scheme
; Tests proper lexical scoping and recursion
(define (A k x1 x2 x3 x4 x5)
  (define (B)
    (set! k (- k 1))
    (A k B x1 x2 x3 x4))
  (if (<= k 0)
      (+ (x4) (x5))
      (B)))

; Should evaluate to -67
(display
 (A
  10
  (lambda () 1)
  (lambda () -1)
  (lambda () -1)
  (lambda () 1)
  (lambda () 0)))
(newline)
```
