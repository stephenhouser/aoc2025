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
#include <unordered_set>

#include "mrf.h"	// map, reduce, filter templates
#include "split.h"	// split strings

using namespace std;

using range_t = pair<size_t, size_t>;

// Hash for pair of size_t's
struct pairhash {
	public:
	  template <typename T, typename U>
	  std::size_t operator()(const std::pair<T, U> &x) const {
		return std::hash<T>()(x.first) ^ std::hash<U>()(x.second);
	  }
	};

/* Update with data type and result types */
using data_t = pair<vector<range_t>, vector<size_t>>;
using result_t = size_t;

/* for pretty printing durations */
using duration_t = chrono::duration<double, milli>;

/* Read the data file... */
const data_t read_data(const string& filename) {
	vector<range_t> ranges;
	vector<size_t> items;

	std::ifstream ifs(filename);

	bool reading_ranges = true;
	string line;
	while (getline(ifs, line)) {
		if (line.empty()) {
			reading_ranges = false;
		} else {
			if (reading_ranges) {
				auto parts = split_size_t(line, "-");
				ranges.push_back({parts[0], parts[1]});
			} else {
				items.push_back(stoul(line));
			}
		}
	}

	return {ranges, items};
}

/* Part 1 */
result_t part1(const data_t& data) {
	const vector<range_t> ranges = data.first;
	const vector<size_t> items = data.second;

	// code version
	// result_t fresh = 0;
	// for (auto item : items) {
	// 	for (const auto& range : ranges) {
	// 		if (in_range(range, item)) {
	// 			fresh++;
	// 			break;
	// 		}
	// 	}
	// }

	// functional version

	// partial function to apply a range to a number
	// Returns true if the number is within the range (inclusive).
	auto contains = [](const size_t n) {
		return [n](const range_t& r) {
			return r.first <= n && n <= r.second;
		};
	};

	// Count the number of items that are in any range
	auto result = ranges::count_if(items, [&ranges, &contains](const auto i) {
		return ranges::any_of(ranges, contains(i));
	});

	return (result_t)result;
}

/* Return `true` if the ranges `r1` and `r2` overlap, false otherwise. */
bool overlaps(const range_t& r1, const range_t& r2) {
	return r1.second > r2.first && r1.first < r2.second;
}

/* Return the merged range of r1 and r2. */
range_t merge(const range_t& r1, const range_t& r2) {
	size_t first  = r1.first  < r2.first  ? r1.first  : r2.first;
	size_t second = r1.second < r2.second ? r2.second : r1.second;
	return {first, second};
}

result_t part2(const data_t& data) {
	unordered_set<range_t, pairhash> check(data.first.begin(), data.first.end());

	// vector of non-overlapping ranges
	vector<range_t> ranges;

	// merge overlapping ranges into `ranges`
	while (check.size()) {
		auto r1 = *(check.begin());
		check.extract(check.begin());

		bool has_overlap = false;
		for (const auto &r2 : check) {
			if (overlaps(r1, r2)) {
				has_overlap = true;
				range_t merger = merge(r1, r2);
				check.extract(r2);
				check.insert(merger);
				break;
			}
		}

		if (!has_overlap) {
			ranges.push_back(r1);
		}
	}

	// sum of lengths of non-overlapping ranges.
	result_t fresh_ids = 0;
	for (const auto& r : ranges) {
		fresh_ids += r.second - r.first + 1;
	}

	return fresh_ids;
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
