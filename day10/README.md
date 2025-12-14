# Day 10: Factory

Solving Set of Linear Equations.

## Part 1

This really looks like solving a system of linear equations.

[.##.] (3) (1,3) (2) (2,3) (0,2) (0,1) {3,5,4,7}
        A    B    C    D     E     F

        [0010] [0101] [0010] [1010] [1100]
            A   B       C       D       E

Use set of all subsets (the power set) and see which one solves.

## Part 2

Solving set of linear eqaequationsutions.

```
       x0  x1  x2  x3  x4  x5     b
A = [   0   0   0   0   1   1 ] [   3 ]
    [   0   1   0   0   0   1 ] [   5 ]
    [   0   0   1   1   1   0 ] [   4 ]
    [   1   1   0   1   0   0 ] [   7 ]
Solution:
x = [   1   5   0   1   3   0 ] <-- sum(10) looking for minimum sum.
```

Hardest one thus far. Spent a lot of time just working out what the linear
equation was! Blaming being busy and getting `1`s and `0`s mixed up. But really
it was just hard.

Gaussian elimination, brute force. Taking way too long.

Broke down and used [z3](https://github.com/Z3Prover/z3) to solve. At least
I got to somewhat learn how to use `z3` in C++ and not just Python (from past year)

Not sure this will rebuild quickly. The `Makefile` has some code to rebuild `z3`.
It takes a while to build.