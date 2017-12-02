;; a simple problem for the Explorer Robot Domain (ewr)
;; 1 rbt, 7 zones + base, 2 tasks and 2 instruments
;; constraint on at most one per robot zone (dom2)

(define (problem ewr2b)
    (:domain ewr2)

  (:objects ra rb - robot
	    base - location
    	    z1 z2 z3 z4 z5 z6 z7 - zone
	    socle brosses - instrument
	    buttage binage - task)

  (:init (adjacent z1 z2) (adjacent z2 z1)
	 (adjacent z2 z3) (adjacent z3 z2)
	 (adjacent z3 z4) (adjacent z4 z3)
	 (adjacent z1 base) (adjacent base z1)
	 (adjacent z4 base) (adjacent base z4)
	 (adjacent z5 base) (adjacent base z5)
	 (adjacent z6 base) (adjacent base z6)
	 (adjacent z7 base) (adjacent base z7)
	 
	 (at ra base) (bare ra)
	 (at rb base) (bare rb)
	 
	 (available socle) (available brosses)
	 
	 (adapted socle binage)
	 (adapted brosses buttage)
	 
	 )

  ;; toutes les zones binage et buttage
  (:goal (and (forall (?z - zone)
		      (and (achieved binage ?z)
			   (achieved buttage ?z)))
	      (at ra base)
	      (at rb base))
	 )
 
  )
	 
	 
