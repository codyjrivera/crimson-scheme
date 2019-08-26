# crimson-scheme -- A recursive Scheme interpreter written in C++.
By Cody Rivera

This is a simple tree-walk intepreter that implements the Scheme programming language.

So far, the basic special forms of Scheme have been implemented: **begin**, **if**,
**define** (variables only), and **set!**, along with a loop construct **while**.

Go to src and type "make" to build, and ./cri-scm to run.

## Sample Programs

Example 1.
```scheme
(begin
  ; variable definitions
  (define x 1)
  (define y 2)
  ; variable swap
  (define temp x)
  (set! x y)
  (set! y temp))
```
