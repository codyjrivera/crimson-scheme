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
