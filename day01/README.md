# Day 1: Secret Entrance

## Part 1

Transform the vector of dial rotations to a vector of positions that result
from applying them. Count the number of zeros in the resulting vector.

For each rotation, if the direction is `L` or negative, increase the next dial
position for each 100 it's under.

## Part 2

Total brain farts on this one, took a while. Brute force is to just turn by one
click each time and look for zeros while keeping in the `0..100` range.

Ugly solution tries to use math to solve more elegantly. I failed at elegance.

Third try is reflecting the dial. If the turn is `L`, transform to `100 - position`
then add the `abs(n)` of the steps to turn. `mod 100` and `100 - position` when 
done. Now the zero crossings is simply `next / 100`.