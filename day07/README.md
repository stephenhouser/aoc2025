# Day 7: Laboratories

## Part 1

For this part we need to count the _activated_ beam splitters `^`.

For each `^` search upwards, if the next non-`.` cell is:

- `^` - stop, this splitter will not be activated.
- `S` - the beam source. This splitter will be activated
- there is a splitter `^` either left or right of the search position, this splitter will be activated.

Count up the activated splitters to get the number of beam splits.

## Part 2

My quick-and-dirty part 1 solution was no use for part 2. In part 2, we need
to count all the unique paths particles could take. I used a simple iterative
solution.

- Keep a vector of active beams on a row.
- On the first row, scan looking for the beam source, put 1 active beam in that position
- For each other row, scan across
 - if we hit a splitter add the incoming (above) beam count to the left and right of the current position
 - if we hit a `.` just add the beam count from directly above (plus what we may have put there from a split).

The active beams will contain the number of possible beam paths.
