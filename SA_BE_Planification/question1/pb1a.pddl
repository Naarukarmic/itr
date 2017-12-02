;; a simple problem for the Explorer Robot Domain (ewr)
;; 1 rbt, 4 zones + base, 3 tasks

(define (problem ewr1a)
    (:domain ewr1)

  (:objects rbt - robot
	    base - location
    	    z1 z2 z3 z4 - zone
	    camera socle brosses pulverisateur - instrument
	    supervision buttage binage epandage - task)

  (:init (adjacent z1 z2) (adjacent z2 z1)
	 (adjacent z2 z3) (adjacent z3 z2)
	 (adjacent z3 z4) (adjacent z4 z3)
	 (adjacent z1 base) (adjacent base z1)
	 (adjacent z4 base) (adjacent base z4)
	 
	 (at rbt base)
	 (bare rbt)

	 (available socle) (available brosses)

	 (adapted camera supervision)
	 (adapted socle binage)
	 (adapted brosses buttage)
	 )

  ;; Le but est de biner toutes les parcelles
  (:goal (and (forall (?z - zone)
  	   		  (achieved binage ?z))
         	  (at rbt base))
  )
)
	 
	 
