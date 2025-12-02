# Day 2: Gift Shop

## Part 1

A regex problem. Faster to use regex or straight code?

Digit sequence repeated **twice**.
Brute force expand all numbers in ranges to strings, and match on `^(.+)\1$`.

transform [start, end] to [all numbers in range]
    filter [all numbers in range] to [repeat numbers]

## Part 2

Digit sequence repeated **twice or more**. Add `+` to the regex match.
Brute force expand all numbers in ranges to strings, and match on `^(.+)\1+$`.
