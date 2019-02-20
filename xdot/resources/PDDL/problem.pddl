(define (problem rosplanquadtask)
(:domain quaddeliver)
(:objects
    q1 - quad
    wp0 wp1 wp2 wp3 wp4 wp5 wp6 wp7 wp10 - waypoint
)
(:init
    (connected wp0 wp1)
    (connected wp1 wp0)
    (connected wp2 wp3)
    (connected wp10 wp3)
    (connected wp3 wp4)
    (connected wp4 wp5)
    (connected wp5 wp6)
    (connected wp6 wp7)
    (connected wp7 wp2)
    (connected wp1 wp10)
    (connected wp10 wp1)
    (doorat wp1 wp2)
    (quadat q1 wp1)

)
(:goal (and
    (quadat q1 wp2)
)))
