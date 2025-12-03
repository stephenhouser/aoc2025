# Day 3: Lobby

## Part 1

For each battery, Search for largest digit between `pos=0` and `size()-1`
in the string. Then search for the largest digit from that point to the end.

`first * 10 + second` is our joltage.

## Part 2

Generalize into a `largest_joltage` function that takes the string, and the
number of digits (2 for part 1 and 12 for part 2). Use the number of digits
to set the `end` search position. Then search for the largest digit from the
start to `end` (size - digits). To find the next digit, set `start` to the
last digit + 1.
