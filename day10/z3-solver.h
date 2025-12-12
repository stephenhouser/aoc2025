#include <iostream>
#include <vector>
#include <z3++.h>

struct z3_solution {
	std::vector<int> x;
	int objectiveValue;
	bool hasSolution = false;
};

extern z3_solution z3_solve_min_sum(const std::vector<std::vector<size_t>>& A, 
									const std::vector<size_t>& b);


// Find multiple solutions
extern std::vector<z3_solution> z3_solve(const std::vector<std::vector<size_t>>& A, 
											const std::vector<size_t>& b, 
											int maxSolutions = 10);
