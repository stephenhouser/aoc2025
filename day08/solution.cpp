/* 2025 Advent of Code - Day 8
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
#include <unordered_set>
#include <map>

#include "point.h"

using namespace std;

/*
	shortest distance in w
*/

/* Update with data type and result types */
using data_t = vector<point_t *>;
using result_t = size_t;

/* circuits are a set of point_t pointers */
using circuit_t = unordered_set<point_t *>;

/* for pretty printing durations */
using duration_t = chrono::duration<double, milli>;

/* Read the data file... */
const data_t read_data(const string& filename) {
	data_t data;

	std::ifstream ifs(filename);

	string line;
	while (getline(ifs, line)) {
		if (!line.empty()) {
			point_t *p = new point_t(point_t::from_string(line));
			data.push_back(p);
		}
	}

	return data;
}

size_t sq_distance(const point_t* p1, const point_t* p2) {
	auto x = (size_t)((p1->x - p2->x) * (p1->x - p2->x));
	auto y = (size_t)((p1->y - p2->y) * (p1->y - p2->y));
	auto z = (size_t)((p1->z - p2->z) * (p1->z - p2->z));	
	return x + y + z;
}

/* Part 1 
	729 too low
*/
/*
162,817,812 and 425,690,689
162,817,812 and 431,825,988 (A)
906,360,560 and 805,96,715 (B)
431,825,988 and 425,690,689 (A)

	5
	4
	2
	2
	1

*/

/* Return a map of the distance between all pairs of points
 * 	distance -> {point, point}
 * Assume no two pairs of points have the same distance
 */
map<size_t, pair<point_t *, point_t *>> point_distances(const data_t &data) {
	map<size_t, pair<point_t *, point_t *>> distance_map;

	for (size_t i = 0; i < data.size(); i++) {
		for (size_t j = i + 1; j < data.size(); j++) {
			auto distance = sq_distance(data[i], data[j]);
			distance_map[distance] = {data[i], data[j]};
		}
	}

	return distance_map;
}

/* Return vector of circuit sizes, sorted largest to smalleset. */
vector<result_t> circuit_sizes(unordered_set<circuit_t *> circuits) {
	// vector<circuit_t *> sizes(circuits.begin(), circuits.end());

	vector<result_t> sizes(circuits.size());
	for (auto circuit : circuits) {
		sizes.push_back((*circuit).size());
	}

	sort(sizes.begin(), sizes.end(), greater());
	return sizes;
}

void add_circuits(const data_t& data, unordered_set<circuit_t *>& circuits) {
	for (auto junction_box : data) {
		circuit_t *circuit = new circuit_t();
		circuit->insert(junction_box);
		circuits.insert(circuit);
	}
}

/* Return the circuit (point set) that contains the point p, NULL otherwise */
circuit_t *find_circuit(unordered_set<circuit_t *>& circuits, point_t *p) {
	for (circuit_t *c : circuits) {
		if (c->contains(p)) {
			return c;
		}
	}

	print("Shit.\n");
	return NULL;
}

/* merge the circuits with p1 and p2 */
void merge_circuits(unordered_set<circuit_t *>& circuits, point_t *p1, point_t *p2) {
	circuit_t *c1 = find_circuit(circuits, p1);
	circuit_t *c2 = find_circuit(circuits, p2);
	if (c1 != c2) {
		for (auto p : *c2) {
			c1->insert(p);
		}

		circuits.erase(c2);
	}
}

result_t part1(const data_t& data) {
	auto distance_map = point_distances(data);

	unordered_set<circuit_t *> circuits;
	add_circuits(data, circuits);

	// based on test vs input data
	int connections = data.size() > 20 ? 1000 : 10;

	for (const auto [distance, points] : distance_map | views::take(connections)) {
		auto [p1, p2] = points;
		merge_circuits(circuits, p1, p2);		
	}

	auto sizes = circuit_sizes(circuits) | views::take(3);
	auto result = accumulate(sizes.begin(), sizes.end(), 1ul, multiplies());
	return result;
}

result_t part2(const data_t& data) {
	auto distance_map = point_distances(data);

	unordered_set<circuit_t *> circuits;
	add_circuits(data, circuits);

	point_t *p1 = NULL;
	point_t *p2 = NULL;
	for (const auto [distance, points] : distance_map) {
		p1 = points.first;
		p2 = points.second;
		merge_circuits(circuits, p1, p2);	
		
		if (circuits.size() == 1) {
			break;
		}
	}

	result_t result = (result_t)p1->x * (result_t)p2->x;
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
