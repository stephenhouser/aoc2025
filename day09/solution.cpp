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

// size_t sq_distance(const point_t& p1, const point_t& p2) {
// 	auto x = (size_t)((p1.x - p2.x) * (p1.x - p2.x));
// 	auto y = (size_t)((p1.y - p2.y) * (p1.y - p2.y));
// 	return x + y;
// }

// size_t rect_area(const point_t& p1, const point_t& p2) {
// 	auto x = (size_t)((p1.x < p2.x) ? (p2.x - p1.x) : (p1.x - p2.x));
// 	auto y = (size_t)((p1.y < p2.y) ? (p2.y - p1.y) : (p1.y - p2.y));
// 	return (x+1) * (y+1);
// }

using box_t = pair<point_t, point_t>;

box_t bounding_box(const vector<point_t> & pts) {
	assert(pts.size() >= 2);

	dimension_t min_x = min(pts[0].x, pts[1].x);
	dimension_t min_y = min(pts[0].y, pts[1].y);
	dimension_t max_x = max(pts[0].x, pts[1].x);
	dimension_t max_y = max(pts[0].y, pts[1].y);

	for (size_t i = 2; i < pts.size(); i++) {
		const point_t& p = pts[i];
		min_x = min(min_x, p.x);
		min_y = min(min_y, p.y);
		max_x = max(max_x, p.x);
		max_y = max(max_y, p.y);	
	}

	return {{min_x, min_y}, {max_x, max_y}};
}

box_t nromalize_box(const point_t &p1, const point_t &p2) {	
	return bounding_box({p1, p2});
}

vector<point_t> box_corners(const box_t& box) {
	vector<point_t> vec;
	vec.push_back({box.first.x,  box.first.y});
	vec.push_back({box.second.x, box.first.y});
	vec.push_back({box.second.x, box.second.y});
	vec.push_back({box.first.x,  box.second.y});
	return vec;
}

size_t box_area(const box_t& b) {
	auto x = (size_t)((b.first.x < b.second.x) ? (b.second.x - b.first.x) : (b.first.x - b.second.x));
	auto y = (size_t)((b.first.y < b.second.y) ? (b.second.y - b.first.y) : (b.first.y - b.second.y));
	return (x+1) * (y+1);
}

vector<box_t> all_boxes(const data_t& data) {
	vector<box_t> rectangles;

	for (size_t i = 0; i < data.size(); i++) {
		for (size_t j = i + 1; j < data.size(); j++) {
			rectangles.emplace_back(nromalize_box(data[i], data[j]));
		}
	}

	return rectangles;
}

/* Part 1 */
result_t part1(const data_t& data) {
	result_t max_area = 0;

	for (const auto& box : all_boxes(data)) {
		auto area = box_area(box);
		// print("{}, {} -> {}\n", box.first, box.second, area);
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


bool same_box(const box_t& a, const box_t &b) {
	return a.first == b.first && a.second == b.second;
}

bool point_in_box(const point_t& p, const box_t &box) {
	return box.first.x < p.x && p.x < box.second.x 
		&& box.first.y < p.y && p.y < box.second.y;
}

/* Return true if the box is completely enclosed by polygon.
 * Assumes box overlaps with polygon.
*/
bool box_in_polygon(const box_t &box, const vector<point_t> &polygon) {
	// we know all the boxes are within the polygon bounds 
	// as they were built from polygon points.

	// Check if any of the polygon points fall inside the box
	// if one does then the box extends outside the polygon
	// for (const auto& p : polygon) {
	// 	if (point_in_box(p, box)) {
	// 		return false;
	// 	}
	// }

	// do any polygon lines cross one of the 4 box borders
	// const auto& corners = box_corners(box);

	for (size_t i = 0; i < polygon.size(); i++) {
		const point_t& a1 = polygon[i];
		const point_t& a2 = polygon[i == polygon.size() ? 0 : i+1];

		if (a1.x == a2.x) {
			auto mn = min(a1.y, a2.y);
			auto mx = max(a1.y, a2.y) + 1;
			for (dimension_t y = mn; y <= mx; y++) {
				if (point_in_box({a1.x, y}, box)) {
					return false;
				}
			}
		} else {
			auto mn = min(a1.x, a2.x);
			auto mx = max(a1.x, a2.x) + 1;
			for (dimension_t x = mn; x <= mx; x++) {
				if (point_in_box({x, a1.y}, box)) {
					return false;
				}
			}
		}
	}


	// 	for (size_t j = 0; j < corners.size(); j++) {
	// 		const point_t& b1 = corners[j];
	// 		const point_t& b2 = corners[j == corners.size() ? 0 : j+1];

	// 		const auto p = intersection(a1, a2, b1, b2);
	// 		if (point_in_box(p, box)) {
	// 			print("{},{}, {},{} intersects at {}\n", a1, a2, b1, b2, p);
	// 			return false;
	// 		} else {
	// 			print("{},{}, {},{} not        at {}\n", a1, a2, b1, b2, p);
	// 		}
	// 	}
	// }

	return true;
}

// bool vector_equals(const vector<point_t>& a, const vector<point_t> &b) {
// 	if (a.size() == b.size()) {
// 		for (size_t i = i; i < a.size(); i++) {
// 			if (a[i] != b[i]) {
// 				return false;
// 			}
// 		}

// 		return true;
// 	}

// 	return false;
// }

// 4,735,268,538
//   180,258,523 too low
// 1,537,458,069
// | grep 1537458069

using line_t = pair<point_t, point_t>;

bool isex(const vector<line_t>& polylines, const vector<line_t>& boxlines, const box_t& box) {
	for (const line_t& pl : polylines) {
		for (const line_t& bl : boxlines) {
			const auto isec = intersection(pl.first, pl.second, bl.first, bl.second);
			if (point_in_box(isec, box)) {
				print("{}", isec);
				return true;
			}
		}
	}
	return false;
}

result_t part2([[maybe_unused]] const data_t& data) {

	// print("\n");
	// for (auto it = box_areas.rbegin(); it != box_areas.rend(); it++) {
	// 	const auto &[area, box] = *it;

	// 	if (box_in_polygon(box, polygon)) {
	// 		if (largest_area < area) {
	// 			largest_area = area;
	// 		}
	// 	}

	// 	// const auto box_vec = box_corners(box);
	// 	// auto clipped = sutherland_hodgman(data, box_vec);
	// 	// print("{} -> {}", area, clipped.size());
	// 	// print_vec(clipped);
	// 	// print("\n");


	// 	// if (clipped.size() == 4) {
	// 	// 	print("\t{}\n", area);
	// 	// }

	// }

	vector<line_t> polylines;
	vector<point_t> polygon;	
	for (size_t i = 0; i < data.size(); i++) {
		const point_t& a1 = data[i];
		const point_t& a2 = data[(i+1) % data.size()];

		auto min_x = min(a1.x, a2.x);
		auto max_x = max(a1.x, a2.x);
		auto min_y = min(a1.y, a2.y);
		auto max_y = max(a1.y, a2.y);
		polylines.push_back({{min_x, min_y}, {max_x, max_y}});

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

	// print("Poly:");
	// for (const auto &pl : polylines) {
	// 	print("{}-{}, ", pl.first, pl.second);
	// }
	// print("\n");

	result_t largest_area = 0;
	for (const auto& box : all_boxes(data)) {
		auto area = box_area(box);
		if (area > largest_area) {
			vector<line_t> boxlines;
			
			const auto corners = box_corners(box);
			for (size_t i = 0; i < corners.size(); i++) {
				const point_t& b1 = corners[i];
				const point_t& b2 = corners[(i+1) % corners.size()];
				boxlines.push_back({b1, b2});
			}

			// print("\tBox:");
			// for (const auto &pl : boxlines) {
			// 	print("{}-{}, ", pl.first, pl.second);
			// }
			// print("\n");
		
			// if (isex(polylines, boxlines, box)) {
			// 	largest_area = area;
			// }

			bool intersects = false;
			for (const point_t& p : polygon) {
				if (box.first.x < p.x && p.x < box.second.x && box.first.y < p.y && p.y < box.second.y) {
					intersects = true;

					// const bool i = isex(polylines, boxlines, box);
					// print("intersects i={} {},{}\n", i, box.first, box.second);

					break;
				}
			}

			if (!intersects) {
				largest_area = area;
			}
		}
	}

	// for (const auto& box : all_boxes(data)) {
	// 	// convert to vector of points
	// 	const auto area = box_area(box);
	// 	print("{:10} ", area);


	// 	auto box_v = box_corners(box);
	// 	print_vec(box_v);
	// 	print("\n");



	// 	auto clipped = sutherland_hodgman(data, box_v);

	// 	if (clipped.size() == 4) {			
	// 		const auto& bound = bounding_box(clipped);

	// 		const auto area = box_area(box);

	// 		if (same_box(box, bound)) {
	// 			// print("{:10} ", area);
	// 			// print_vec(box_v);
	// 			// print(": ");
	// 			// print_vec(clipped);
	// 			// print("\n");

	// 			if (largest_area < area) {
	// 				largest_area = area;
	// 			}
	// 		}
	// 	}
	// }

	return largest_area;
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
