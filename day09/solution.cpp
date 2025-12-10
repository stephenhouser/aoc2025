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

#include "point.h"
#include "vector.h"

using namespace std;

/* Update with data type and result types */
using data_t = vector<point_t>;
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
			data.push_back(point_t::from_string(line));
		}
	}

	return data;
}

struct line_t : vector_t {
	line_t(const point_t& a, const point_t& b) : vector_t(a, b) {}
};

struct rectangle_t {
	point_t p1;
	point_t p2;

	rectangle_t(const point_t& a, const point_t& b) : p1(), p2() {
		p1.x = min(a.x, b.x);
		p1.y = min(a.y, b.y);
		p2.x = max(a.x, b.x);
		p2.y = max(a.y, b.y);
	}

	rectangle_t(const tuple<point_t, point_t>& tp) : p1(), p2() {
		p1.x = min(get<0>(tp).x, get<1>(tp).x);
		p1.y = min(get<0>(tp).y, get<1>(tp).y);
		p2.x = max(get<0>(tp).x, get<1>(tp).x);
		p2.y = max(get<0>(tp).y, get<1>(tp).y);
	}

	dimension_t area() const {
		auto x = (p1.x < p2.x) ? (p2.x - p1.x) : (p1.x - p2.x);
		auto y = (p1.y < p2.y) ? (p2.y - p1.y) : (p1.y - p2.y);
		return (x+1) * (y+1);
	}

	bool contains(const point_t& p) const {
		return (p1.x < p.x && p.x < p2.x &&
				p1.y < p.y && p.y < p2.y);
	}
};

/* Returns a vector of all the rectangles formed by the vector of points
 */
vector<rectangle_t> all_rectangles(const data_t& data) {
	vector<rectangle_t> rectangles;

	for (size_t i = 0; i < data.size(); i++) {
		for (size_t j = i + 1; j < data.size(); j++) {
			const rectangle_t rectangle(data[i], data[j]);
			rectangles.push_back(rectangle);
		}
	}

	return rectangles;
}

/* Part 1 */
result_t part1(const data_t& data) {
	// All pairs of points to rectangles
	const auto areas = views::cartesian_product(data, data)
		| views::transform([](const auto& t) { return rectangle_t(t); })
		| views::transform([](const rectangle_t& r) { return r.area(); });

	// Get the largest area rectangle
	return (result_t)ranges::max(areas);
}

/* Returns the midpoint of the line described by p1 and p2. */
point_t midpoint(const point_t& p1, const point_t& p2) {
	const auto p = p1 + p2;
	return {p.x/2, p.y/2};
}

/* Returns true if the rectangle is inside the polygon described by
 * the vector of points. False otherwise.
 */
bool enclosed_rectangle(const rectangle_t& r, const data_t& polygon) {
	for (size_t i = 0; i < polygon.size(); i++) {
		const point_t& p1 = polygon[i];

		// check starting point of line
		if (r.contains(p1)) {
			return false;
		}

		// check mid-point of line
		const point_t& p2 = polygon[(i+1) % polygon.size()];
		const point_t p(midpoint(p1, p2));
		if (r.contains(p)) {
			return false;
		}
	}

	// all points of polygon were outside the rectangle
	// and no lines of the polygon intersected it
	return true;
}

/* Part 2 */
result_t part2(const data_t& data) {
	// All pairs of points to rectangles

	// Cannot seem to get the filter() working correctly.
	// const auto areas = views::cartesian_product(data, data)
	// 	| views::transform([](const auto& t) { return rectangle_t(t); })
	// 	| views::filter([&data](const rectangle_t& r) {
	// 			return enclosed_rectangle(r, data);
	// 		})
	// 	| views::transform([&data](const rectangle_t& r) { 
	// 			return r.area(); 
	// 		});

	const auto areas = views::cartesian_product(data, data)
		| views::transform([](const auto& t) { return rectangle_t(t); })
		| views::transform([&data](const rectangle_t& r) { 
			return enclosed_rectangle(r, data) ? r.area() : 0; 
		});
		
	// Get the largest area rectangle
	return (result_t)ranges::max(areas);
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
