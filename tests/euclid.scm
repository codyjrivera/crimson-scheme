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
