;; a simple problem for the Explorer Robot Domain (ewr)
;; 2 rbt + uav, 4 zones + base, 4 tasks and 3 instruments
;; constraint on at most one per robot zone
;; 1 rbt + 1 uav, 4 zones + base, 4 tasks and 3 instruments
;; constraint on at most one per robot zone
;; requirement constraints on tasks
;; energy constraint on vehicles

(define (problem ewr5a)
  (:domain ewr5)

  (:objects ra - robot
	    drn - uav
	    base - location
    	    z1 z2 z3 z4 - zone
	    camera socle brosses pulverisateur - instrument
	    supervision buttage binage epandage - task)



  (:init (adjacent z1 z2) (adjacent z2 z1)
	 (adjacent z2 z3) (adjacent z3 z2)
	 (adjacent z3 z4) (adjacent z4 z3)
	 (adjacent z1 base) (adjacent base z1)
	 (adjacent z4 base) (adjacent base z4)
	 
	 (at ra base) (bare ra)
	 (at drn base) (full drn)
	 

	 (available socle)
	 (available brosses)
	 (fixed ra camera)
	 (fixed drn pulverisateur)
	 (fixed drn camera)
	 
	 (adapted camera supervision)
	 (adapted socle binage)
	 (adapted brosses buttage)
	 (adapted pulverisateur epandage)

	 (independent supervision)
	 (independent binage)
         (require buttage binage)
         (require epandage supervision)

	 )

   ;; Le but est de butter et d'epandre, avec precedence, toutes les parcelles
  (:goal (and (forall (?z - zone)
  	      (and 
	         (achieved epandage ?z)
		 (achieved buttage ?z)
		 ))		 
	      (at ra base)
	      (at drn base))
	 )
  )
	 
	 
