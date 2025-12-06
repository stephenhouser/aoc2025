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

/* Read the data file... */
const data_t read_data(const string& filename) {
	data_t data;

	std::ifstream ifs(filename);

	vector<string> lines;

	string line;
	while (getline(ifs, line)) {
		if (!line.empty()) {
			lines.push_back(line);
		}
	}

	// find column end positions
	vector<size_t> columns;
	columns.push_back(0);
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
	columns.push_back(lines[0].size());

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

	// for (const auto& eq : data) {
	// 	for (const auto& term : eq) {
	// 		print("[{}] ", term);
	// 	}
	// 	print("\n");
	// }

	return data;
}


// // Trim from start
// inline string ltrim(const string &s) {
// 	string t(s);
//     t.erase(t.begin(), std::find_if(t.begin(), t.end(),
//             std::not1(std::ptr_fun<int, int>(std::isspace))));
//     return t;
// }

// // Trim from the end
// inline string rtrim(const string &s) {
// 	string t(s);
//     t.erase(std::find_if(t.rbegin(), t.rend(),
//             std::not1(std::ptr_fun<int, int>(std::isspace))).base(), t.end());
//     return t;
// }

// Trim from both ends
inline string trim(const string &src) {
	size_t start = 0;
	while (start < src.size() && isspace(src[start])) {
		start++;
	}

	size_t end = src.size();
	while (end != 0 && isspace(src[end])) {
		end--;
	}

	string t = src.substr(start, end - start);
    return t;
}

vector<vector<string>> transpose(const vector<vector<string>>& src) {
	size_t rows = src.size();
	size_t columns = src[0].size();

	vector<vector<string>> dst;
	for (size_t c = 0; c < columns; c++) {
		vector<string> dst_row;
		for (size_t r = 0; r < rows; r++) {
			dst_row.push_back(src[r][c]);
		}

		dst.push_back(dst_row);
	}

	return dst;
}

result_t solver_p1(const vector<string>& eq) {
	char op = eq[eq.size()-1][0];

	size_t local = (op == '+') ? 0 : 1;

	for (size_t i = 0; i < eq.size() - 1; i++) {
		string t = trim(eq[i]);
		size_t value = stoul(trim(eq[i]));
		local = (op == '+') ? (local + value): (local * value);
	}

	return local;
}

/* Part 1 */
result_t part1(const data_t& data) {

	result_t result = 0;
	for (auto& eq : data) {
		const auto local = solver_p1(eq);
		result += local;
	}

	return result;
}

const vector<string> trans(const vector<string>& src) {
	size_t rows = src.size();
	size_t columns = src[0].size();

	vector<string> dst;
	for (size_t c = 0; c < columns; c++) {
		string dst_str;
		for (size_t r = 0; r < rows; r++) {
			dst_str += src[r][c];
		}

		dst.push_back(dst_str);
	}

	return dst;
}

result_t solver_p2(const vector<string>& eq) {
	char op = eq[eq.size()-1][0];
	size_t local = (op == '+') ? 0 : 1;

	vector<string> neq = eq;
	neq.pop_back();
	vector<string> xp = trans(neq);

	for (const auto& term : xp) {
		print("[{}] ", term);
	}

	print("\n");
 

	for (size_t i = 0; i < xp.size(); i++) {
		string t = trim(xp[i]);
		size_t value = stoul(trim(xp[i]));
		local = (op == '+') ? (local + value): (local * value);
	}


	// for (size_t i = 0; i < eq.size() - 1; i++) {
	// 	const string& term = eq[i];
	// 	for (size_t col = 0; col < term.size(); col++) {
	// 		const char ch = term[col];
	// 		if (!isspace(ch)) {
	// 			const size_t digit = ch - '0';

	// 		}
	// 	}

	// 	string t = trim(eq[i]);
	// 	size_t value = stoul(trim(eq[i]));
	// 	local = (op == '+') ? (local + value): (local * value);
	// }

	return local;
}

result_t part2(const data_t& data) {
	result_t result = 0;
	for (auto& eq : data) {
		const auto local = solver_p2(eq);
		result += local;
	}

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
