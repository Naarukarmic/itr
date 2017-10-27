;; Specification of the Environment Worker Robot (ewr) Domain
;; constraint on at most one robot per zone, does not apply to "base"
;; constraint on order of tasks: either independent or have precedence requirements
;; constraint on available energy, recharge at base only
;; has ground robots + UAVs



(define (domain ewr5)
  
  (:types 
   location	   ; either a zone or the "base" location
   zone - location ; several connected zones, at most 1 robot per zone
   vehicle	   ;  either a robot or a uav
   robot - vehicle ; carries at most 1 instrument
   uav - vehicle ; has a fixed camera, at most one "survey" task per trip
   instrument	 ; used by a robot to achieve tasks
   task		 ; activity to be achieved in various zones
   )

  (:predicates
   (adjacent ?z1  ?z2 - location)    ; location ?z1 is adjacent to ?z2
   (at ?x - vehicle ?z - location)   ; robot ?x is at location ?z
   (occupied ?z - location)	     ; there is a robot at zone ?z
   (carry ?r - vehicle ?i - instrument) ; robot ?r carries an instrument ?i
   (bare ?r - robot)		      ; robot ?r has no instrument
   (full ?u - uav)	; uav ?u is at base and tanf full for an epandage task
   (achieved ?t - task ?z - zone) ; task ?t in zone ?z has been achieved
   (adapted ?i - instrument ?t - task) ; instrument ?i can be used for task ?t
   (available ?i - instrument)	      ; instrument ?i is available at the base
   (fixed ?r - vehicle ?i - instrument) ; instrument fixed on the robot or UAV
   (independent ?t - task)	      ; task ?t can be performed any time
   (require ?t ?tbefore - task) ; ?t requires that ?t before to be achieved earlier on same zone
   )

  ;; there are 5 operators in this domain:

  ;; move a robot between two adjacent zones, discharge its battery

  ;; mount instrument?i on robot at base base

  ;; unmount instrument ?i from robot at base base
 
  ;; perform task ?t at zone ?z by robot ?r with instrument ?i


  ;; spray task ?t (= epandage) at zone ?z by robot ?r with instrument ?i
  
  ;; fly uav ?u flies from zone/base to zone/base

  ;; refill the drone tank at the base

  )


