;; Specification of the Environment Worker Robot (ewr) Domain
;; constraint on at most one robot per zone, does not apply to "base"
;; constraint on order of tasks: either independent or have precedence requirements

(define (domain ewr3)
  
    (:types 
     location		   ; either a zone or the "base" location
     zone	- location ; several connected zones, at most 1 robot per zone
     robot		   ; carries at most 1 instrument
     instrument		   ; used by a robot to achieve tasks
     task		   ; activity to be achieved in various zones
     )

  (:predicates
   (adjacent ?z1  ?z2 - location)    ; location ?z1 is adjacent to ?z2
   (at ?x - robot ?z - location)     ; robot ?x is at location ?z
   (occupied ?z - location)	     ; there is a robot at zone ?z
   (carry ?r - robot ?i - instrument) ; robot ?r carries an instrument ?i
   (bare ?r - robot)		      ; robot ?r has no instrument
   (achieved ?t - task ?z - zone) ; task ?t in zone ?z has been achieved
   (adapted ?i - instrument ?t - task) ; instrument ?i can be used for task ?t
   (available ?i  - instrument)	      ; instrument ?i is available at the base
   (fixed ?r - robot ?i - instrument) ; instrument fixed on the robot
   )


;; there are 4 operators in this domain:
  ;; move a robot between two adjacent zones
  (:action move
    :parameters (?r - robot ?from ?to - location)
    :precondition (and (adjacent ?from ?to)
                       (at ?r ?from)
                       (or (= ?to base)
                           (not (occupied ?to))
                       )
    )
    :effect (and (at ?r ?to)
                 (not (at ?r ?from))
                 (occupied ?to)
                 (not (occupied ?from))
    )
  )
  ;; mount instrument ?i on robot at base base
  (:action mount
    :parameters (?r - robot ?i - instrument)
    :precondition (and (at ?r base)
                       (available ?i)
                       (bare ?r)
    )
    :effect (and (carry ?r ?i)
                 (not (bare ?r))
                 (not (available ?i))
    )
  )
  ;; unmount instrument ?i from robot at base base
  (:action unmount
    :parameters (?r - robot ?i - instrument)
    :precondition (and (at ?r base)
                       (carry ?r ?i)
    )
    :effect (and (not (carry ?r ?i))
                 (bare ?r)
                 (available ?i)
    )
  )
  ;; perform task ?t at zone ?z by robot ?r with instrument ?i
  (:action perform
    :parameters (?r - robot ?z - zone ?i - instrument ?t - task)
    :precondition (and (at ?r ?z)
                       (or (carry ?r ?i)
                           (fixed ?r ?i)
                       )
                       (adapted ?i ?t)
                       (not (achieved ?t ?z))
    )
    :effect (achieved ?t ?z)
  )
)



