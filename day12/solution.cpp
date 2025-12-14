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

struct package_t {
	size_t id = 0;
	string layout = "";

	size_t area() const {
		return accumulate(layout.begin(), layout.end(), 0ul, 
			[](size_t acc, const char c) {
				return (c =='#') ? acc + 1 : acc;
			});
	}
};

struct space_t {
	size_t width = 0;
	size_t height = 0;
	vector<size_t> package_counts = {};

	size_t area() const {
		return height * width;
	}

	size_t needed_area(const map<size_t, package_t>& packages) const {
		size_t needed = 0;

		for (size_t pid = 0; pid < package_counts.size(); pid++) {
			if (packages.contains(pid)) {
				const package_t& package = packages.at(pid);
				needed += package.area() * package_counts[pid];
			}
		}

		return needed;
	}
};


/* Update with data type and result types */
using data_t = pair<std::map<size_t, package_t>, vector<space_t>>;
using result_t = size_t;

/* for pretty printing durations */
using duration_t = chrono::duration<double, milli>;

/* split at last empty line*/
const string slurp(const string& filename) {
	std::ifstream ifs(filename);
	std::ostringstream str;
	str << ifs.rdbuf();
	return str.str();
}

/* Read the data file... */
const data_t read_data(const string& filename) {
	std::ifstream ifs(filename);

	vector<string> chunks;
	string chunk;

	string line;
	while (getline(ifs, line)) {
		if (!line.empty()) {
			chunk.append("\n" + line);
		} else {
			chunks.push_back(chunk);
			chunk.clear();
		}
	}

	// trailing chunk is the spaces, parse the spaces
	vector<space_t> spaces;
	for (const auto& space : split(chunk, "\n")) {
		if (space.size() > 1) {
			const auto parts = split(space, "x: ");
			
			space_t space;
			space.height = stoul(parts[0]);
			space.width = stoul(parts[1]);
			for (size_t s = 2; s < parts.size(); s++) {
				space.package_counts.push_back(stoul(parts[s]));
			}

			spaces.push_back(space);			
		}
	}

	// parse packages
	std::map<size_t, package_t> packages;
	for (size_t i = 0; i < chunks.size(); i++) {
		const string& s = chunks[i];

		package_t package;

		const auto colon = chunks[i].find(':');

		package.id = stoul(s.substr(0, colon));
		package.layout = s.substr(colon+2);
		packages[package.id] = package;
	}

	return {packages, spaces};
}

/* Part 1 */
result_t part1(const data_t& data) {
	const auto& packages = data.first;
	const auto& spaces = data.second;



	auto ok_spaces = ranges::count_if(spaces, [&packages](const space_t& space) {
			return space.area() >= space.needed_area(packages);
		});

	return (result_t)ok_spaces;
}

result_t part2([[maybe_unused]] const data_t& data) {
	// The surprise part 2
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
