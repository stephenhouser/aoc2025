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

#include "point.h"
#include "charmap.h"

using namespace std;

/* Update with data type and result types */
using data_t = charmap_t;
using result_t = size_t;

/* for pretty printing durations */
using duration_t = chrono::duration<double, milli>;

/* Read the data file... */
const data_t read_data(const string& filename) {
	return charmap_t::from_file(filename);
}

/* Part 1 */
result_t part1(const data_t& map) {
	const point_t up(0, -1);
	const point_t left(-1, 0);
	const point_t right(1, 0);

	result_t active_splitters = 0;

	for (point_t p : map.all_points('^')) {
		// print("{}:{}: ", p, map.get(p));
		p = p + up;
		while (map.is_valid(p)) {
			// print("up={}:{} ", p, map.get(p));
			if (map.is_char(p, '^')) {
				break;
			}

			if (map.is_char(p, 'S')) {
				active_splitters += 1;
				break;
			}

			// print("left={}:{}, right={}:{} ", p+left, map.get(p+left), p+right, map.get(p+right));
			if (map.is_char(p + left, '^') || map.is_char(p + right, '^')) {
				active_splitters += 1;
				break;
			}

			p = p + up;
		}

		// print("\n");
	}

	return active_splitters;
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
