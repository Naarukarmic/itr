;; a simple problem for the Explorer Robot Domain (ewr)
;; 1 rbt, 7 zones + base, 3 tasks and 2 instruments
;; constraint on at most one per robot zone
;; requirement constraints on tasks


(define (problem ewr3b)
    (:domain ewr3)

  (:objects rbt - robot
	    base - location
    	    z1 z2 z3 z4 z5 z6 z7 - zone
	    camera socle brosses - instrument
	    supervision buttage binage  - task)

  (:init (adjacent z1 z2) (adjacent z2 z1)
	 (adjacent z2 z3) (adjacent z3 z2)
	 (adjacent z3 z4) (adjacent z4 z3)
	 (adjacent z1 base) (adjacent base z1)
	 (adjacent z4 base) (adjacent base z4)
	 (adjacent z5 base) (adjacent base z5)
	 (adjacent z6 base) (adjacent base z6)
	 (adjacent z7 base) (adjacent base z7)
	 
	 (at rbt base) (bare rbt)

	 (available socle) (available brosses)
	 (fixed rbt camera)
	 
	 (adapted camera supervision)
	 (adapted socle binage)
	 (adapted brosses buttage)

	 )

  ;; Le but est de biner, superviser et butter toutes les parcelles
  (:goal (and (forall (?z - zone)
  	      (and (achieved binage ?z)
	           (achieved supervision ?z)
		   (achieved buttage ?z)))
	      (at rbt base))
	      
	 )
  )
	 
	 
