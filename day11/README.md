# Day 11: Reactor

Recursion with Cache (Dynamic Programming).

## Part 1

Trace all the paths and count the number that end at `out`.

## Part 2

Even went with a `dot` graph to see what this looked like. Checking for loops
mostly. Had some code to check for loops, just in case. Was able to remove.

Every path from svr to out via dac and fft. This is the same as every path
from `srv -> fft -> dac -> out` times all the paths `srv -> dac -> fft -> out`.
So I can just add these together.
