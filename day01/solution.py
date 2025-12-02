#!/usr/bin/env python3
#
# 2025 Advent of Code, Day 1
# 2025-12-02 Stephen Houser <stephenhouser@gmail.com>
# 
import time     # for performance_counter()
import sys      # for sys.argv

def read_input(file: str):
    """Read input data file, return list of turns"""

    data = []

    with open(file, "r", encoding="utf-8") as inf:
        for line in inf.readlines():
            direction = -1 if line[0] == "L" else 1
            data.append(direction * int(line[1:]))

    return data

def turn_dial(dial, steps):
    """Turns the dial from start by steps and returns the new position"""

    # reverse the world on left turns
    dial = (100 - dial if steps < 0 else dial) % 100

    # turn the dial, count zero crossings and set the new dial position
    next = dial + abs(steps)
    zeros = next // 100
    dial = next % 100

    # reverse the world back on left turns
    dial = (100 - dial if steps < 0 else dial) % 100
    return (dial, zeros)

def part1(turns, start):
    """Returns the number of times the dial is left on 0 after turning."""

    dial = start
    zeros = 0

    for steps in turns:
        (dial, _) = turn_dial(dial, steps)

        if dial == 0:
            zeros += 1

    return zeros

def part2(turns, start):
    """Returns the number of times the dial crosses 0 while turning."""

    dial = start
    zero_crossings = 0

    for steps in turns:
        (dial, zeros) = turn_dial(dial, steps)
        zero_crossings += zeros

    return zero_crossings


def main():
    if len(sys.argv) == 1:
        print("No input file given")
        exit(1)

    start_time = time.perf_counter()
    data = read_input(sys.argv[1])
    parse_time = time.perf_counter()

    p1 = part1(data, 50)
    p1_time = time.perf_counter()

    p2 = part2(data, 50)
    p2_time = time.perf_counter()

    total_time = (p2_time - start_time) * 1000
    p2_time = (p2_time - p1_time) * 1000
    p1_time = (p1_time - parse_time) * 1000
    parse_time = (parse_time - start_time) * 1000

    verbose = "\n"

    if verbose != "":
        print(f"{'parse':>15} ({parse_time:>10.4f}ms) ", end=verbose)

    print(f"{p1:>15} ({p1_time:>10.4f}ms) ", end=verbose)
    print(f"{p2:>15} ({p2_time:>10.4f}ms) ", end=verbose)
    print(f"{'total':>15} ({total_time:>10.4f}ms) ")


if __name__ == "__main__":
    main()