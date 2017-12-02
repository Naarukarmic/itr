;; Specification of the Environment Worker Robot (ewr) Domain
;; constraint on at most one robot per zone, does not apply to "base"
;; constraint on order of tasks: either independent or have precedence requirements
;; has ground robots + UAVs



(define (domain ewr5)
  
  (:types 
   location    ; either a zone or the "base" location
   zone - location ; several connected zones, at most 1 robot per zone
   vehicle     ;  either a robot or a uav
   robot - vehicle ; carries at most 1 instrument
   uav - vehicle ; has a fixed camera, at most one "survey" task per trip
   instrument  ; used by a robot to achieve tasks
   task    ; activity to be achieved in various zones
   )

  (:predicates
   (adjacent ?z1  ?z2 - location)    ; location ?z1 is adjacent to ?z2
   (at ?x - vehicle ?z - location)   ; robot ?x is at location ?z
   (occupied ?z - location)      ; there is a robot at zone ?z
   (carry ?r - vehicle ?i - instrument) ; robot ?r carries an instrument ?i
   (bare ?r - robot)          ; robot ?r has no instrument
   (full ?u - uav)  ; uav ?u is at base and tank full for an epandage task
   (achieved ?t - task ?z - zone) ; task ?t in zone ?z has been achieved
   (adapted ?i - instrument ?t - task) ; instrument ?i can be used for task ?t
   (available ?i - instrument)        ; instrument ?i is available at the base
   (fixed ?r - vehicle ?i - instrument) ; instrument fixed on the robot or UAV
   (independent ?t - task)        ; task ?t can be performed any time
   (require ?t ?tbefore - task) ; ?t requires that ?t before to be achieved earlier on same zone
   )

  ;; there are 5 operators in this domain:

  ;; move a robot between two adjacent zones
  (:action move
  :parameters (?r - robot ?from ?to - location)
  :precondition(and (adjacent ?from ?to)
      (at ?r ?from)
      (or (= ?to base)
        (not(occupied ?to))) )
  :effect(and(at ?r ?to)
      (not (at ?r ?from))
      (not (occupied ?from))
      (occupied ?to) )
  )

  ;; mount instrument ?i on robot at base base
  (:action mount
  :parameters(?r - robot ?i - instrument)
  :precondition(and(at ?r base)
      (available ?i)
      (bare ?r))
  :effect(and(carry ?r ?i)
    (not (bare ?r))
    (not (available ?i)) )
  )

  ;; unmount instrument ?i from robot at base base
  (:action unmount
  :parameters(?r - robot ?i - instrument)
  :precondition(and(at ?r base)
      (carry ?r ?i))
  :effect(and(not(carry ?r ?i))
    (bare ?r)
    (available ?i) )
  )
 
  ;; perform task ?t at zone ?z by vehicle ?v with instrument ?i
   (:action perform
  :parameters(?v - vehicle ?t - task ?i - instrument ?z - zone)
  :precondition(and(at ?v ?z)
      (not (= ?t epandage))
      (or (fixed ?v ?i)(carry ?v ?i))
      (adapted ?i ?t)
      (not(achieved ?t ?z)) 
      (or (independent ?t)
        (and(achieved binage ?z)(require ?t binage)) ))
  :effect(achieved ?t ?z)
  )

  ;; spray task ?t (= epandage) at zone ?z by robot ?r with instrument ?i
  (:action spray
  :parameters(?u - uav ?z - zone)
  :precondition(and(at ?u ?z)
      (fixed ?u pulverisateur)
      (not(achieved epandage ?z)) 
      (achieved supervision ?z)
      (full ?u)
      (not(occupied ?z)) )
  :effect(and(achieved epandage ?z)
      (not(full ?u)) )
  )

  ;; fly uav ?u flies from zone/base to zone/base
  (:action fly
  :parameters (?u - uav ?from ?to - location)
  :precondition(at ?u ?from)
  :effect(and(at ?u ?to)
      (not (at ?u ?from)) )
  )


  ;; refill the drone tank at the base
  (:action refill
  :parameters (?u - uav)
  :precondition(and(at ?u base)
      (not(full ?u)))
  :effect(full ?u)
  )

)


