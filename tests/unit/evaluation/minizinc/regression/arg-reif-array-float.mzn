% RUNS ON mzn20_fd
var 1..3: a;

predicate test_pred(array[int] of 1.0..10.0: x, var int: y) = y = 3;

constraint test_pred([1.0, 2.0, 4.0, 11.0], a) == false;

solve satisfy;

output ["a = ", show(a), ";\n"];
