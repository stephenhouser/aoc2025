# Day 5: Cafeteria

## Part 1

Read ranges and items. Count the number of items that are in one (or more) of
the ranges. Make sure to count each item only once.

## Part 2

Merge the ranges and then return the total number of possible fresh items.

merged = {} -- ranges with no overlaps
check = ranges -- ranges with overlaps, starts as all ranges

while check not empty
    r1 = check.pop()
    has_overlap with other = false
    for r1 in check -- check for overlap with another range
        if overlaps(r1, r2)
            has_overlap = true
            merger = merge(r1, r2)  -- merge the two
            check.add(merged)       -- add back to be checked with others
            break;                  -- don't check more, these are invalid

    if !has_overlap -- if we didn't see an overlap, this one is done
        merged.add(r)

