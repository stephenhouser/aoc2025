# Advent of Code 2025

| Day | Part 1 | Part 2 | Title |
|---:|:-----:|:-----:|:-----|
| 1  | ⭐ | 🌟 | [Day 1: Secret Entrance](https://adventofcode.com/2025/day/1) |
| 2  | ⭐ | 🌟 | [Day 2: Gift Shop](https://adventofcode.com/2025/day/2) |
| 3  | ⭐ | 🌟 | [Day 3: Lobby](https://adventofcode.com/2025/day/3) |
| 4  | ⭐ | 🌟 | [Day 4: Printing Department](https://adventofcode.com/2025/day/4) |
| 5  | ⭐ | 🌟 | [Day 5: Cafeteria](https://adventofcode.com/2025/day/5) |
| 6  | ⭐ | 🌟 | [Day 6: Trash Compactor](https://adventofcode.com/2025/day/6) |
| 7  | ⭐ | 🌟 | [Day 7: Laboratories](https://adventofcode.com/2025/day/7) |
| 8  | ⭐ | 🌟 | [Day 8: Playground](https://adventofcode.com/2025/day/8) |
| 9  | ⭐ | 🌟 | [Day 9: Movie Theater](https://adventofcode.com/2025/day/9) |
| 10 | ⭐ | 🌟 | [Day 10: Factory](https://adventofcode.com/2025/day/10) |
| 11 | ⭐ | 🌟 | [Day 11: Reactor](https://adventofcode.com/2025/day/11) |
| 12 | ⭐ | 🌟 | [Day 12: Christmas Tree Farm](https://adventofcode.com/2025/day/12) |

Contains my solutions to the [Advent of Code 2025](https://adventofcode.com/2025).

Tools:
             2
- C++ 23 (STL only)
- Visual Studio Code
- macOS / Linux (primary)
- git

- [z3](https://github.com/Z3Prover/z3) for Day 10, because it sucked otherwise.

## Running

There is a top level makefile that can be used to make each day or a `summary`

- `make day05` will make day 05 and test against the live `input.txt` in that directory.
- `make summary` will make all days and run against the `test.txt` and `input.txt` in each directory.

When developing, within each day's directory.

- `make` or `make test` will build and run `test.txt` which is sample input from the problem
- `make input` will build and run `input.txt` which is the live input for the problem

To debug, change the `Makefile`, remove `-O3` and replace with `-g`. Then use GDB
or modify the launch configuration in `.vscode/launch.json` for the appropriate day and input file.

## archlinux

```
day00:              * (    0.0000ms)              * (    0.0200ms)          total (    0.0549ms)
day01:           **** (    0.0124ms)           **** (    0.0332ms)          total (    0.2167ms)
day02:    *********** (   18.5591ms)    *********** (   49.0821ms)          total (   67.6893ms)
day03:          ***** (    0.0138ms)*************** (    0.0435ms)          total (    0.1185ms)
day04:           **** (    0.2738ms)           **** (   12.6686ms)          total (   12.9942ms)
day05:            *** (    0.0776ms)*************** (    0.0481ms)          total (    0.2219ms)
day06:  ************* (    0.0694ms)  ************* (    0.1037ms)          total (    0.3455ms)
day07:           **** (    0.0213ms) ************** (    0.0415ms)          total (    0.1481ms)
day08:         ****** (  124.4493ms)      ********* (  160.4342ms)          total (  285.0625ms)
day09:     ********** (    0.3187ms)     ********** (   75.4402ms)          total (   75.8471ms)
day10:            *** (   63.7722ms)          ***** (  445.0638ms)          total (  509.2563ms)
day11:            *** (    0.0467ms)*************** (    0.8868ms)          total (    1.1796ms)
day12:            *** (    0.0353ms)              * (    *.0143ms)          total (    0.4699ms)
```
