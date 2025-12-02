/* 2025 Advent of Code - Day 1
 *
 * Stephen Houser <stephenhouser@gmail.com>
 * All rights reserved.
 */

#include <getopt.h>	 // getopt

#include <algorithm>  // sort
#include <cassert>	  // assert macro
#include <chrono>	  // high resolution timer
#include <cstring>	  // strtok, strdup
#include <fstream>	  // ifstream (reading file)
#include <numeric>	  // max, reduce, etc.
#include <print>
#include <ranges>  // ranges and views
#include <string>  // strings
#include <vector>  // collectin

using namespace std;

/* Update with data type and result types */
using data_t = vector<int>;
using result_t = size_t;

/* for pretty printing durations */
using duration_t = chrono::duration<double, milli>;

/* Read the data file... */
const data_t read_data(const string& filename) {
	data_t data;

	std::ifstream ifs(filename);

	string line;
	while (getline(ifs, line)) {
		if (!line.empty()) {
			switch(line[0]) {
				case 'L':
					data.push_back(-stoi(line.substr(1)));
					break;
				case 'R':
					data.push_back(stoi(line.substr(1)));
					break;
				default:
					break;
			}
		}
	}

	return data;
}

/* Part 1 */
result_t part1(const data_t& data) {
	int position = 50;

	auto turn_dial = [&position](int n) {
		auto next = position;
		if (n < 0) {
			next += 100 * ((-n / 100) + 1);
		}
		position = (next + n) % 100;
		return position;
	};

	auto positions = data | views::transform(turn_dial);
	return (result_t)ranges::count(positions, 0);
}

result_t part2(const data_t& data) {
	int position = 50;

	/* Brute force solution */
	[[maybe_unused]]
	auto brute_force = [&position](int n) {
		auto zeros = 0;
		for (auto ticks = abs(n); ticks > 0; ticks--) {
			if (n < 0) {
				// + 100 to stay above 0
				position = (position - 1 + 100) % 100;
			} else {
				position = (position + 1) % 100;
			}

			zeros += position == 0 ? 1 : 0;
		}

		return zeros;
	};

	/* Original ugly math-y solution */
	[[maybe_unused]]
	auto ugly_crosses = [&position](int n) {
		auto next = position + n;

		if (next == 0) {
			position = next;
			return 1;
		}

		if (next < 0) {
			auto crossing = position == 0 ? -1 : 0;
			while (next < 0) {
				next += 100;
				crossing++;
			}

			crossing += next == 0 ? 1 : 0;

			position = next;
			return crossing;
		}

		auto crossing = (next / 100);
		position = next % 100;

		return crossing;
	};

	/* The math-y solution */
	auto elegant_crosses = [&position](int n) {
		if (n < 0) {
			position = 100 - position;
		}

		auto next = (position % 100) + abs(n);
		auto cross = next / 100;

		position = next % 100;

		if (n < 0) {
			position = 100 - position;
		}

		return cross;
	};

	auto crossings = data | views::transform(elegant_crosses);
	return std::accumulate(crossings.begin(), crossings.end(), 0u, std::plus());
}

int main(int argc, char* argv[]) {
	bool verbose = false;

	int c;
	while ((c = getopt(argc, argv, "v")) != -1) {
		switch (c) {
			case 'v':
				verbose = !verbose;
				break;
			default:
				std::print(stderr, "ERROR: Unknown option \"{}\"\n", c);
				exit(1);
		}
	}

	argc -= optind;
	argv += optind;

	const char* input_file = argv[0];
	if (argc != 1) {
		std::print(stderr, "ERROR: No input file specified\n");
		exit(2);
	}

	auto start_time = chrono::high_resolution_clock::now();

	auto data = read_data(input_file);

	auto parse_complete = chrono::high_resolution_clock::now();
	duration_t parse_time = parse_complete - start_time;
	if (verbose) {
		print("{:>15} ({:>10.4f}ms)\n", "parse", parse_time.count());
	}

	result_t p1_result = part1(data);

	auto p1_complete = chrono::high_resolution_clock::now();
	duration_t p1_time = p1_complete - parse_complete;
	print("{:>15} ({:>10.4f}ms){}", p1_result, p1_time.count(), verbose ? "\n" : "");

	result_t p2_result = part2(data);

	auto p2_complete = chrono::high_resolution_clock::now();
	duration_t p2_time = p2_complete - p1_complete;
	print("{:>15} ({:>10.4f}ms){}", p2_result, p2_time.count(), verbose ? "\n" : "");

	duration_t total_time = p2_complete - start_time;
	print("{:>15} ({:>10.4f}ms)\n", "total", total_time.count());
}
