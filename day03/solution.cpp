#include <getopt.h>	 // getopt

#include <cassert>	  // assert macro
#include <chrono>	  // high resolution timer
#include <cstring>	  // strtok, strdup
#include <fstream>	  // ifstream (reading file)
#include <numeric>	  // max, reduce, etc.
#include <print>
#include <ranges>  // ranges and views
#include <string>  // strings
#include <vector>  // collection

using namespace std;

/* Update with data type and result types */
using data_t = vector<string>;
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
			data.push_back(line);
		}
	}

	return data;
}

/* Return index of largest digit in string (s) between start and end positions */
size_t largest_digit(const string& s, size_t start, size_t end) {
	assert(start < end);		// end is after start
	assert(end <= s.size());	// end is not beyond the end of the string

	size_t largest_idx = start;	// assume first is largest
	for (size_t i = start + 1; i < end; i++) {
		if (s[i] > s[largest_idx]) {
			largest_idx = i;
		}
	}

	return largest_idx;
}

/* Return the largest joltage with the correct number of digits. */
size_t largest_joltage(const string&battery, const size_t digits) {
	size_t joltage = 0;
	size_t start = 0;

	size_t last_pos = battery.size() - (digits - 1);
	for (size_t end = last_pos; end <= battery.size(); end++) {
		auto digit_idx = largest_digit(battery, start, end);
		joltage = (joltage * 10) + (size_t)battery[digit_idx] - '0';
		start = digit_idx + 1;
	}

	return joltage;
}

/* Part 1 */
result_t part1(const data_t& data) {
	/* Accumulate largest 2 digit joltages */
	auto two_digit_joltage = [](result_t a, const string& battery) {
		return a + largest_joltage(battery, 2);
	};

	result_t result = accumulate(data.begin(), data.end(), 0ul, two_digit_joltage);
	return result;
}

result_t part2(const data_t& data) {
	/* Accumulate largest 12 digit joltages */
	auto twelve_digit_joltage = [](result_t a, const string& battery) {
		return a + largest_joltage(battery, 12);
	};

	result_t result = accumulate(data.begin(), data.end(), 0ul, twelve_digit_joltage);
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
