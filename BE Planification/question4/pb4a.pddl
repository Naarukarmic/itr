;; a simple problem for the Explorer Robot Domain (ewr)
;; 1 rbt + 1 uav, 4 zones + base, 3 tasks and 3 instruments
;; constraint on at most one per robot zone
;; requirement constraints on tasks


(define (problem ewr4a)
    (:domain ewr4)

  (:objects rbt - robot
	    base - location
    	    z1 z2 z3 z4 - zone
	    camera socle brosses - instrument
	    supervision buttage binage - task)

  (:init (adjacent z1 z2) (adjacent z2 z1)
	 (adjacent z2 z3) (adjacent z3 z2)
	 (adjacent z3 z4) (adjacent z4 z3)
	 (adjacent z1 base) (adjacent base z1)
	 (adjacent z4 base) (adjacent base z4)
	 
	 (at rbt base) (bare rbt)

	 (available socle)
	 (available brosses)
	 (fixed rbt camera)
	 
	 (adapted camera supervision)
	 (adapted socle binage)
	 (adapted brosses buttage)

	 (independent supervision)
	 (independent binage)
         (require buttage binage)
	 
	 )

   ;; Le but est de butter, avec precedence toutes les parcelles
  (:goal (and (forall (?z - zone)
  	      (and (achieved supervision ?z)
	           (achieved buttage ?z)
		   ))
	      (at rbt base))
	 )
  )