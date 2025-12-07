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

using namespace std;

/* Update with data type and result types */
using data_t = vector<vector<string>>;
using result_t = size_t;

/* for pretty printing durations */
using duration_t = chrono::duration<double, milli>;


/* Find column (like tab stops) where all lines have a space character. */
vector<size_t> find_columns(const vector<string>& lines) {
	vector<size_t> columns;

	// first column starts at index 0.
	columns.push_back(0);

	// search for index where all lines have a space character
	for (size_t i = 0; i < lines[0].size(); i++) {
		bool is_col = true;
		for (const string& line : lines) {
			if (line[i] != ' ') {
				is_col = false;
				break;
			}
		}

		if (is_col) {
			columns.push_back(i);
		}	
	}

	// add end of line as last column marker.
	columns.push_back(lines[0].size());

	return columns;
}

/* Split the lines at column positions and return vector of column data. */
data_t split_columns(const vector<string>& lines, const vector<size_t>& columns) {
	data_t data;

	for (size_t c = 0; c < columns.size()-1; c++) {
		auto start = columns[c];
		start = (start == 0) ? start : start + 1;

		auto len = columns[c+1] - start;

		vector<string> local;
		for (const auto& line : lines) {
			local.push_back(line.substr(start, len));
		}

		data.push_back(local);
	}

	return data;
}

/* Read the data file... */
const data_t read_data(const string& filename) {
	vector<string> lines;

	std::ifstream ifs(filename);

	string line;
	while (getline(ifs, line)) {
		if (!line.empty()) {
			lines.push_back(line);
		}
	}

	const vector<size_t> columns = find_columns(lines);
	const data_t data = split_columns(lines, columns);
	return data;
}

// Trim from both ends
// using the below toul() instead
// inline string trim(const string &src) {
// 	size_t start = 0;
// 	while (start < src.size() && isspace(src[start])) {
// 		start++;
// 	}

// 	size_t end = src.size();
// 	while (end != 0 && isspace(src[end])) {
// 		end--;
// 	}

// 	string t = src.substr(start, end - start);
//     return t;
// }

inline result_t toul(const string& src) {
	// advanced past leading spaces
	size_t start = 0;
	while (start < src.size() && isspace(src[start])) {
		start++;
	}

	return stoul(src.substr(start));
}
 
/* Returns result of equation, + or * all the terms, based on operator in last element */
result_t solve(const vector<string>& eq) {
	char op = eq[eq.size()-1][0];
	assert(op == '+' || op == '*');

	size_t result = (op == '+') ? 0 : 1;

	for (size_t i = 0; i < eq.size() - 1; i++) {
		size_t value = toul(eq[i]);

		result = (op == '+') ? (result + value) 
							 : (result * value);
	}

	return result;
}

/* Part 1 */
result_t part1(const data_t& data) {
	result_t result = accumulate(data.begin(), data.end(), 0ul, 
		[](result_t a, const vector<string>& eq) {
			return a + solve(eq);
		}
	);

	return result;
}

/* Transpose equation for part 2 */
const vector<string> transpose_eq(const vector<string>& src) {
	size_t rows = src.size();
	size_t columns = src[0].size();

	vector<string> dst;
	for (size_t c = 0; c < columns; c++) {
		string dst_str;

		// Don't transpose last row (that's where the +/* is)
		// We will add it back in at the end...
		for (size_t r = 0; r < rows - 1; r++) {
			dst_str += src[r][c];
		}

		dst.push_back(dst_str);
	}

	// add the operation back at the end, un-transposed.
	dst.push_back(src[src.size()-1]);
	return dst;
}

result_t part2(const data_t& data) {
	result_t result = accumulate(data.begin(), data.end(), 0ul, 
		[](result_t a, const vector<string>& eq) {
			return a + solve(transpose_eq(eq));
		}
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
