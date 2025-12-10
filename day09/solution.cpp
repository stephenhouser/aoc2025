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
#include <set>

#include "point.h"
#include "vector.h"
#include "sutherland-hodgeman.h"

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

	dimension_t area() const {
		auto x = (p1.x < p2.x) ? (p2.x - p1.x) : (p1.x - p2.x);
		auto y = (p1.y < p2.y) ? (p2.y - p1.y) : (p1.y - p2.y);
		return (x+1) * (y+1);
	}

	bool is_inside(const point_t& p) const {
		return (p1.x < p.x && p.x < p2.x &&
				p1.y < p.y && p.y < p2.y);
	}

	vector<point_t> corners() const {
		vector<point_t> vec;
		vec.push_back({p1.x, p1.y});
		vec.push_back({p2.x, p1.y});
		vec.push_back({p2.x, p2.y});
		vec.push_back({p1.x, p2.y});
		return vec;
	}

	vector<line_t> edges() const {
		vector<line_t> vec;

		auto corner_vec = corners();
		for (size_t i = 0; i < corner_vec.size(); i++) {
			vec.push_back({corner_vec[i], corner_vec[(i+1) % corner_vec.size()]});
		}

		return vec;
	}

	bool operator==(const rectangle_t& other) const {
		return (p1 == other.p1 && p2 == other.p2);
	}
};

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
	result_t max_area = 0;

	for (const auto& box : all_rectangles(data)) {
		auto area = (result_t)box.area();
		if (max_area < area) {
			max_area = area;
		}
	}

	return max_area;
}

void print_vec(const vector<point_t>& vec) {
	for (const auto& p : vec) {
		print("{} ", p);
	}
}

bool is_outside(const rectangle_t& r, const point_t& p) {
	return p.x <= r.p1.x || r.p2.x <= p.x ||
		   p.y <= r.p1.y || r.p2.y <= p.y;
}

bool sh_box_inside(const rectangle_t& box, const data_t& polygon) {
	// if all red spots are outside or on line
	for (const auto& p : polygon) {
		if (!is_outside(box, p)) {
			return false;
		}	
	}

	return true;
}

/* Part 2 */
result_t part2(const data_t& data) {
	result_t largest_area = 0;

	for (const auto& box : all_rectangles(data)) {
		auto area = (result_t)box.area();

		// if (largest_area < area) {
			print("{:4} {},{}: clip=", area, box.p1, box.p2);
			if (sh_box_inside(box, data)) {
				print("***");
				largest_area = area;
			}
			print("\n");
		// }		
	}

	return largest_area;
	
#if 0
	for (const auto& box : all_rectangles(data)) {
		auto area = (result_t)box.area();

		if (largest_area < area /*|| area == 1537458069 */) {
			// const auto clip = sutherland_hodgman(data, box.corners());
			const auto clip = sutherland_hodgman(box.corners(), data);
			print("{:4} {},{}: clip=", area, box.p1, box.p2);
			print_vec(box.corners());

			print(" result=");
			print_vec(clip);

			if (same_rectangle(clip, box) /*|| area ==  1537458069*/ ) {
				print(" is same");
				largest_area = area;
			}
			print("\n");
			// if (is_rectangle(clip) || area == 1537458069) {
			// 	const auto fix = to_rectangle(clip);
			// 	print("{:4} {},{}: ", area, box.p1, box.p2);
			// 	print_vec(clip);
			// 	print(" is rectangle {},{}", fix.p1, fix.p2);

			// 	if (fix == box) {
			// 		largest_area = area;
			// 		print(" ***");
			// 	}
			// }
			// print("\n");
		}
	}

	return largest_area;
#endif

#if 0
	// generate all points on the polygon edges
	vector<point_t> polygon;	
	for (size_t i = 0; i < data.size(); i++) {
		const point_t& a1 = data[i];
		const point_t& a2 = data[(i+1) % data.size()];

		if (a1.y == a2.y) {
			auto mn = min(a1.x, a2.x);
			auto mx = max(a1.x, a2.x);
			for (dimension_t x = mn; x <= mx; x++) {
				polygon.push_back({x, a1.y});
			}
		} else {
			auto mn = min(a1.y, a2.y);
			auto mx = max(a1.y, a2.y);
			for (dimension_t y = mn; y <= mx; y++) {
				polygon.push_back({a1.x, y});
			}
		}
	}

	result_t largest_area = 0;

	for (const auto& box : all_rectangles(data)) {
		auto area = (result_t)box.area();

		if (largest_area < area) {
			if (contains_box(polygon, box)) {
				largest_area = area;
			}
		}
	}

	return largest_area;
#endif
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
