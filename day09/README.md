# Day 9: Movie Theater

Geometry, Intersecting Lines, Point in Polygon

## Part 1

Get the list of all pairs of points and make rectangles from them, choose the
largest one.

Refined to use cartesian product to generate the pairs (ranges),
transform them to areas, then find the max() of those.

## Part 2

OOf. This was a hard one

Get all possible rectangles, as in part 1, then test each one to see if it
is fully enclosed (this was the hard part). Of the fully enclosed ones,
find the max sized one.

The fully enclosed was rough. Looking for largest interior polygon.
Tried using Sutherland-Hodgman for clipping. Idea was to clip the large data
polygon to each box and see if the result was the "same" as the box used to clip.
This didn't quite work because some larger boxes would clip to the same box?

Tried a few line intersects box. Solved by making the outer polygon a vector of
all the points on the outer polygon. Then for each box, seeing if any of those
points landed inside it. This was the slow, ugly version.

Refined by using a simple line intersects line function and created a
`enclosed_rectangle()` based on that. March through the points of the polygon
(outer data) and see if the line from one point to the next crossed the rectangle
border. If so, the rectangle is not fully enclosed, reject it.

Refined further to be more "functional"

This one was a long haul.
