# Day 8: Playground

## Part 1

Keep shortest distance among pairs of nodes in a vector, sort it. Don't `sqrt`
the distances, not needed to get relative distance.

Make a collection of circuits from the junction boxes, one circuit per box.
Take distance pairs in shortest to longest order, find which circuit each
box is in, merge those two circuits (remove one, add to the other). Do that 1000 times.

Calculate the size of all the circuits and sort by size, take top 3 and multiply for result.

## Part 2

Same as part 1, but track which boxes are joined and iterate until there is only
one circuit. The last two boxes joined are our result, multiply their X values.
