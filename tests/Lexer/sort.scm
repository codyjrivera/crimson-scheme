#| This will test the multiline commenting facility of my lexer |#

(define (partition compare l1)
  (cond
   ((null? l1) '())
   ((compare (car l1)) (cons (car l1) (partition compare (cdr l1))))
   (else (partition compare (cdr l1)))))

(define (quicksort l1)
  (cond
   ((null? l1) '())
   (else (let ((pivot (car l1))) #| This will probably be some rambling comment that
           probably really shouldn't be in the code but I am using it to test the 
           lexer |#
           (append (append (quicksort (partition (lambda (x) (< x pivot)) l1))
                           (partition (lambda (x) (= x pivot)) l1))
                   (quicksort (partition (lambda (x) (> x pivot)) l1)))))))
