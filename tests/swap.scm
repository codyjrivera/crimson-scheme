; variable definitions
(define x 1)
(define y 2)

(display "x ")
(display x)
(newline)

(display "y ")
(display y)
(newline)

; variable swap
(define temp x)
(set! x y)
(set! y temp)


(display "x ")
(display x)
(newline)

(display "y ")
(display y)
(newline)
