/* 2025 Advent of Code - Day 2
 *
 * Stephen Houser <stephenhouser@gmail.com>
 * All rights reserved.
 */
#include <getopt.h>	 	// getopt

#include <algorithm>  	// sort
#include <cassert>	  	// assert macro
#include <chrono>	  	// high resolution timer
#include <cstring>	  	// strtok, strdup
#include <fstream>	  	// ifstream (reading file)
#include <numeric>	  	// max, reduce, etc.
#include <print>		// formatted print
#include <ranges>  		// ranges and views
#include <string>  		// strings
#include <vector>  		// collection
#include <climits>
#include <set>

#include "mrf.h"	// map, reduce, filter templates
#include "split.h"	// split strings

using namespace std;

/* Update with data type and result types */
struct machine_t {
	size_t display;
	vector<size_t> buttons;
	vector<size_t> requirements;
};

using data_t = vector<machine_t>;

/* for pretty printing durations */
using duration_t = chrono::duration<double, milli>;
using result_t = size_t;

size_t parse_display(const string& s) {
	size_t display = 0x00;
	for (auto it = s.rbegin(); it != s.rend(); it++) {
		const char c = *it;
		if (c == '.') {
			display = display << 1 | 0x00;
		} else if (c == '#') {
			display = display << 1 | 0x01;
		}
	}

	return display;
}

size_t parse_button(const string& s) {
	size_t button = 0x00;
	for (const auto pos : split_size_t(s, "(),")) {
		button |= (0x01 << pos);
	}

	return button;
}

/* Read the data file... */
const data_t read_data(const string& filename) {
	data_t data;

	std::ifstream ifs(filename);

	string line;
	while (getline(ifs, line)) {
		if (!line.empty()) {
			auto parts = split(line, " ");
			assert(parts.size() >= 3);

			size_t display = parse_display(parts[0]);
			vector<size_t> buttons;
			for (size_t i = 1; i < parts.size()-1; i++) {
				buttons.emplace_back(parse_button(parts[i]));
			}

			vector<size_t> requirements = split_size_t(parts[parts.size()-1], "{},");
			data.push_back({display, buttons, requirements});
		}
	}

	return data;
}

set<set<size_t>> powerset(const vector<size_t>& src) {
	set<set<size_t>> pow;

	for (const auto i : src) {
		for (const auto& s : pow) {
			set<size_t> ss(s.begin(), s.end());
			ss.insert(i);
			pow.insert(ss);
		}
		pow.insert({i});
	}

	return pow;
}

/* Part 1 */
result_t part1(const data_t& data) {

	result_t result = 0;

	for (const auto& machine : data) {
		result_t local_min = LONG_MAX;

		// try every possible combination of single button presses
		// e.g. the powerset of buttons
		for (const auto& s : powerset(machine.buttons)) {
			size_t display = 0x00;

			// only try if the size of the set is smaller than our current min
			if (s.size() < local_min) {
				// each button is an xor
				for (const auto i : s) {
					display ^= i;
				}

				// if the result is the same as the display, save as new min
				if (display == machine.display) {
					local_min = s.size();
				}
			}
		}

		result += local_min;
	}

	return result;
}

result_t part2([[maybe_unused]] const data_t& data) {
	return 0;
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
