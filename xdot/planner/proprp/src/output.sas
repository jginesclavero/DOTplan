begin_version
3
end_version
begin_metric
1
end_metric
4
begin_variable
var0
-1
2
Atom connected(wp1, wp2)
NegatedAtom connected(wp1, wp2)
end_variable
begin_variable
var1
-1
2
Atom connected(wp2, wp1)
NegatedAtom connected(wp2, wp1)
end_variable
begin_variable
var2
-1
2
Atom doorat(wp1, wp2)
NegatedAtom doorat(wp1, wp2)
end_variable
begin_variable
var3
-1
9
Atom quadat(q1, wp0)
Atom quadat(q1, wp1)
Atom quadat(q1, wp10)
Atom quadat(q1, wp2)
Atom quadat(q1, wp3)
Atom quadat(q1, wp4)
Atom quadat(q1, wp5)
Atom quadat(q1, wp6)
Atom quadat(q1, wp7)
end_variable
1
begin_mutex_group
9
3 0
3 1
3 2
3 3
3 4
3 5
3 6
3 7
3 8
end_mutex_group
begin_state
1
1
0
1
end_state
begin_goal
1
3 3
end_goal
15
begin_operator
move-to q1 wp0 wp1
0
1
0 3 0 1
6
end_operator
begin_operator
move-to q1 wp1 wp0
0
1
0 3 1 0
6
end_operator
begin_operator
move-to q1 wp1 wp10
0
1
0 3 1 2
6
end_operator
begin_operator
move-to q1 wp1 wp2
1
0 0
1
0 3 1 3
6
end_operator
begin_operator
move-to q1 wp10 wp1
0
1
0 3 2 1
6
end_operator
begin_operator
move-to q1 wp10 wp3
0
1
0 3 2 4
6
end_operator
begin_operator
move-to q1 wp2 wp1
1
1 0
1
0 3 3 1
6
end_operator
begin_operator
move-to q1 wp2 wp3
0
1
0 3 3 4
6
end_operator
begin_operator
move-to q1 wp3 wp4
0
1
0 3 4 5
6
end_operator
begin_operator
move-to q1 wp4 wp5
0
1
0 3 5 6
6
end_operator
begin_operator
move-to q1 wp5 wp6
0
1
0 3 6 7
6
end_operator
begin_operator
move-to q1 wp6 wp7
0
1
0 3 7 8
6
end_operator
begin_operator
move-to q1 wp7 wp2
0
1
0 3 8 3
6
end_operator
begin_operator
opendoor_DETDUP_0_WEIGHT_1 q1 wp1 wp2
2
2 0
3 1
2
0 0 -1 0
0 1 -1 0
90
end_operator
begin_operator
opendoor_DETDUP_1_WEIGHT_1 q1 wp1 wp2
1
3 1
3
0 0 -1 1
0 1 -1 1
0 2 0 1
90
end_operator
0
