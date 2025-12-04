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

const auto accessible_bales(const data_t& data) {
	/* Can the given location be accessed by the forklift?
	 * Does it have fewer than four (4) neighboring rolls of paper '@'?
	 */
	const auto can_access = [&data](const point_t& p) {
		auto n =  ranges::count_if(data.neighbors_of(p), [](const auto &pr) { return pr.second == '@'; });
		return n < 4;
	};

	/* Filter all the rolls of paper by which ones we can access. */
	return data.all_points('@') | views::filter(can_access);
}

/* Part 1 
* Result is the number of accessible paper rolls.
* NOTE: filter view is not sized_range so need to use ::distance not size()
*/
result_t part1(const data_t& data) {
	auto accessible = accessible_bales(data);
	return (result_t)ranges::distance(accessible);
}

/* Utility to clear temporary 'x' where we removed a bale of paper */
void clear_x(data_t& map) {
	for (const auto& p : map.all_points('x')) {
		map.set(p, '.');
	}
}

/* Remove accessible bales of paper, update the map */
result_t remove_bales(data_t& map) {
	// return number of bales removed
	auto accessible = accessible_bales(map);
	auto removed =  (result_t)ranges::distance(accessible);

	// need to realize the filter to a vector, otherwise it
	// dynamically updates as we clear locations
	for (const auto &p : accessible | ranges::to<vector<point_t>>()) {
		map.set(p, 'x');
	}

	return removed;
}

result_t part2(const data_t& data) {
	result_t total_removed = 0;

	data_t map = data;	// non-const version so we can update it
	result_t removed = 0;
	do {
		removed = remove_bales(map);
		// only need to clear when we are debugging, 'x' is not '@' so all is good
		//clear_x(map);

		total_removed += removed;
	} while (removed != 0);

	return total_removed;
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
