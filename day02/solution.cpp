/* 2025 Advent of Code - Day 2
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
#include <vector>  // collection
#include <regex>

#include "split.h"	// split strings

using namespace std;

/* Update with data type and result types */
using data_t = vector<pair<size_t, size_t>>;
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
			auto id_ranges = split(line, ",");
			for (const auto& id_range : id_ranges) {
				auto parts = split_size_t(id_range, "-");
				if (parts.size() == 2) {
					data.push_back({parts[0], parts[1]});
				}
			}
		}
	}

	return data;
}

/* Part 1 */
result_t part1(const data_t& data) {
	/* Old school version with for loops. Not much faster */
	/*
	result_t result = 0;
	for (const auto& [start, end] : data) {
		print("{} : {}\n", start, end);

		for (size_t n = start; n <= end; n++) {
			if (is_twice_repeat(n)) {
				result += n;
			}
		}
	}

	return result;
	*/

	const auto is_invalid = [](const size_t n) {
		return regex_match(to_string(n), regex("^(.+)\\1$"));
	};
	
	result_t result = ranges::fold_left(
		data | views::transform([&is_invalid](const pair<size_t, size_t>& p) {
			return ranges::fold_left(
				views::iota(p.first, p.second+1) | views::filter(is_invalid),
				0ul, std::plus());
			}), 
			0ul, std::plus()
		);

	return result;
}

result_t part2(const data_t& data) {
	const auto is_invalid = [](const size_t n) {
		return regex_match(to_string(n), regex("^(.+)\\1+$"));
	};
	
	result_t result = ranges::fold_left(
		data | views::transform([&is_invalid](const pair<size_t, size_t>& p) {
			return ranges::fold_left(
				views::iota(p.first, p.second+1) | views::filter(is_invalid),
				0ul, std::plus());
			}), 
			0ul, std::plus()
		);

	return result;
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
