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
#include <map>

#include "split.h"	// split strings

using namespace std;

template <>
struct std::formatter<vector<string>> {
	constexpr auto parse(std::format_parse_context& context) {
		return context.begin();
	}

	auto format(const vector<string>& vec, std::format_context& context) const {
		auto out = context.out();

		for (size_t i = 0; i < vec.size(); i++) {
			auto sep = (i < vec.size()-1) ? ", " : "";
			std::format_to(out, "{}{}", vec[i], sep);
		}

		return out;
	}
};

/* Update with data type and result types */
using data_t = std::map<string, vector<string>>;
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
			auto kv = split(line, ":");
			auto values = split(kv[1]);

			data[kv[0]] = values;
		}
	}

	return data;
}

/* Return the number of paths to get from `start` to `end` */
// result_t count_paths(const data_t& network, const string& start, const string& end) {
// 	if (start == end) {
// 		return 1;
// 	}

// 	result_t paths = 0;
// 	for (const auto& next : network.at(start)) {
// 		paths += count_paths(network, next, end);
// 	}

// 	return paths;
// }

std::map<string, size_t> cache;

/* Return the number of paths to get from `start` to `end` */
result_t count_paths(const data_t& network, const string& start, const string& end) {
	string key = start + end;
	if (cache.contains(key)) {
		return cache.at(key);
	}

	if (start == end) {
		return 1;
	}

	if (!network.contains(start)) {
		return 0;
	}

	result_t paths = 0;
	for (const auto& next : network.at(start)) {
		paths += count_paths(network, next, end);
	}

	cache[key] = paths;
	return paths;
}

/* Part 1 */
result_t part1(const data_t& data) {
	cache.clear();

	// this works for input data
	if (data.contains("you")) {
		return count_paths(data, "you", "out");
	}

	return count_paths(data, "svr", "out");
}

result_t part2(const data_t& data) {
	cache.clear();

	if (data.contains("svr")) {
		result_t to_dac = count_paths(data, "svr", "dac")
						* count_paths(data, "dac", "fft")
						* count_paths(data, "fft", "out");

		result_t to_fft = count_paths(data, "svr", "fft")
						* count_paths(data, "fft", "dac")
						* count_paths(data, "dac", "out");

		return to_dac + to_fft;
	}

	return count_paths(data, "you", "out");
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
