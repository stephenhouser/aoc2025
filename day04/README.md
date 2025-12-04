# Day 4: Printing Department

## Part 1

A good exercise in using `charmap_t` and functional programming!

Select all the points on the map that have `@` and filter on those that
have less than four neighbors. These are the `accessible` rolls of paper.
Count them with `std::distance()`.

NOTE: cannot use `.size()` on a range that is not `sized_range` and filters
are not. Had to use `std::distance()` to get the size.

## Part 2

Similar to part 1, just remove the identified `accessible` rolls of paper and
iterate until there are no accessible rolls. Keep track of the total rolls
removed.

Interestingly ran into a problem where `distance` counted the number of rolls
correctly, but then when setting all the accessible ones to '.' (or 'x')
there were more accessible ones. This is because the `filter` is dynamic,
not static. So when a location is set to 'x' it creates new accessible rolls.

Solution was to *realize* the filtered accessible bales as a vector and then
iterate over that vector. Thus making what was dynamic, static.
