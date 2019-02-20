(define (domain quaddeliver)
(:requirements :typing)
(:types quad waypoint - object)

(:predicates
  (connected ?wp1 ?wp2 - waypoint)
  (quadat ?q - quad ?wp - waypoint)
  (doorat ?from ?to - waypoint)
)

(:action move-to
 :parameters (?q - quad ?wp1 ?wp2 - waypoint)
 :precondition (and (quadat ?q ?wp1) (connected ?wp1 ?wp2))
 :effect (and
    (not (quadat ?q ?wp1))
    (quadat ?q ?wp2)
  )
)

(:action opendoor
  :parameters (?q - quad ?from ?to - waypoint)
  :precondition (and (quadat ?q ?from) (doorat ?from ?to))
  :effect (and
    (probabilistic
      0.8 (and (connected ?from ?to) (connected ?to ?from))
      0.2 (and (not(doorat ?from ?to)) (not(connected ?from ?to)) (not(connected ?to ?from)))
    )
  )
)

)
