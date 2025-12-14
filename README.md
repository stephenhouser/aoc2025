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
| 12 |    |    | [Day 12: Christmas Tree Farm](https://adventofcode.com/2025/day/12) |

Contains my solutions to the [Advent of Code 2025](https://adventofcode.com/2025).

Tools:

- C++ 23 (STL only)
- Visual Studio Code
- macOS / Linux (primary)
- git

- [z3]() for Day 10, because it sucked otherwise.

## Running

There is a top level makefile that can be used to make each day or a `summary`

- `make day05` will make day 05 and test against the live `input.txt` in that directory.
- `make summary` will make all days and run against the `test.txt` and `input.txt` in each directory.

When developing, within each day's directory.

- `make` or `make test` will build and run `test.txt` which is sample input from the problem
- `make input` will build and run `input.txt` which is the live input for the problem

To debug, change the `Makefile`, remove `-O3` and replace with `-g`. Then use GDB
or modify the launch configuration in `.vscode/launch.json` for the appropriate day and input file.
