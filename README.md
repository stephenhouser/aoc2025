# Advent of Code 2025

| Day | Part 1 | Part 2 | Title |
|---:|:-----:|:-----:|:-----|
| 1  | ⭐ | 🌟 | [Day 1: Secret Entrance](https://adventofcode.com/2025/day/1) |
| 2  | ⭐ | 🌟 | [Day 2: Gift Shop](https://adventofcode.com/2025/day/2) |
| 3  | ⭐ | 🌟 | [Day 3: Lobby](https://adventofcode.com/2025/day/3) |
| 4  | ⭐ | 🌟 | [Day 4: Printing Department](https://adventofcode.com/2025/day/3) |
| 5  | ⭐ | 🌟 | [Day 5: Cafeteria](https://adventofcode.com/2025/day/3) |
| 6  |    |    |    |
| 7  |    |    |    |
| 8  |    |    |    |
| 9  |    |    |    |
| 10 |    |    |    |
| 11 |    |    |    |
| 12 |    |    |    |

Contains my solutions to the [Advent of Code 2025](https://adventofcode.com/2025).

Tools:

- C++ 23 (STL only)
- Visual Studio Code
- macOS / Linux (primary)
- git

## Running

There is a top level makefile that can be used to make each day or a `summary`

- `make day05` will make day 05 and test against the live `input.txt` in that directory.
- `make summary` will make all days and run against the `test.txt` and `input.txt` in each directory.

When developing, within each day's directory.

- `make` or `make test` will build and run `test.txt` which is sample input from the problem
- `make input` will build and run `input.txt` which is the live input for the problem

To debug, change the `Makefile`, remove `-O3` and replace with `-g`. Then use GDB
or modify the launch configuration in `.vscode/launch.json` for the appropriate day and input file.
