# Day 6: Trash Compactor

## Part 1

This one really seems to be a parsing problem. Most of the work is in parsing
the data to columns instead of rows. Once that is done, a simple equation solver
(for `+` and `*`) returns the results.

## Part 2

Again, a parsing problem, with a little 2D array manipulation.
Adjusted part 1 to find where the column breaks were then split the input,
keeping leading and trailing spaces. Each equation is `transposed` and then,
solved as in part 1.