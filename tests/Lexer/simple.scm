;; Should skip this comment

(define x 2)
(define y 3)

(set! y x)

(if (> x y)
    'this-shouldnt-happen
    #t)

(+ (+ 1 2 3) 4 (* 2 y))
