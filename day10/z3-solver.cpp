#include <iostream>
#include <vector>
#include <z3++.h>
#include "z3-solver.h"

z3_solution z3_solve_min_sum(const std::vector<std::vector<size_t>>& A, 
								const std::vector<size_t>& b) {
	z3_solution result;
	
	size_t m = A.size();    // number of equations
	size_t n = A[0].size(); // number of variables
	
	// Create Z3 context
	z3::context ctx;
	z3::optimize opt(ctx);
	// parallel is not any faster...
	// z3::set_param("parallel.enable", true);
	// z3::set_param("parallel.threads.max", 16);

	// Create integer variables x[0], x[1], ..., x[n-1]
	std::vector<z3::expr> x;
	for (size_t i = 0; i < n; i++) {
		std::string name = "x" + std::to_string(i);
		x.push_back(ctx.int_const(name.c_str()));
		
		// Add non-negativity constraint
		opt.add(x[i] >= 0);
	}
	
	// Add equality constraints Ax = b
	for (size_t i = 0; i < m; i++) {
		z3::expr constraint = ctx.int_val(0);
		for (size_t j = 0; j < n; j++) {
			if (A[i][j] != 0) {  // Only add non-zero coefficients
				constraint = constraint + ctx.int_val(static_cast<int>(A[i][j])) * x[j];
			}
		}
		opt.add(constraint == ctx.int_val(static_cast<int>(b[i])));
	}
	
	// Set objective: minimize sum of all variables
	z3::expr objective = ctx.int_val(0);
	for (size_t i = 0; i < n; i++) {
		objective = objective + x[i];
	}
	opt.minimize(objective);
	
	// Solve
	z3::check_result check_result = opt.check();
	
	if (check_result == z3::sat) {
		result.has_solution = true;
		
		z3::model model = opt.get_model();
		result.x.resize(n);

		// accumulate result from solution
		result.value = 0;
		for (size_t i = 0; i < n; i++) {
			z3::expr val = model.eval(x[i]);
			result.x[i] = val.get_numeral_int();
			result.value += result.x[i];
		}
	}
	
	return result;
}

// Find multiple solutions
std::vector<z3_solution> z3_solve(const std::vector<std::vector<size_t>>& A, 
									const std::vector<size_t>& b, 
									int max_solutions) {
	std::vector<z3_solution> solutions;
	
	size_t m = A.size();
	size_t n = A[0].size();
	
	z3::context ctx;
	z3::solver solver(ctx);
	
	// Create variables
	std::vector<z3::expr> x;
	for (size_t i = 0; i < n; i++) {
		std::string name = "x" + std::to_string(i);
		x.push_back(ctx.int_const(name.c_str()));
		solver.add(x[i] >= 0);
	}
	
	// Add constraints
	for (size_t i = 0; i < m; i++) {
		z3::expr constraint = ctx.int_val(0);
		for (size_t j = 0; j < n; j++) {
			if (A[i][j] != 0) {
				constraint = constraint + ctx.int_val(static_cast<int>(A[i][j])) * x[j];
			}
		}
		solver.add(constraint == ctx.int_val(static_cast<int>(b[i])));
	}
	
	// Find multiple solutions
	int count = 0;
	while (count < max_solutions && solver.check() == z3::sat) {
		z3_solution sol;
		sol.has_solution = true;
		
		z3::model model = solver.get_model();
		sol.x.resize(n);
		
		// Extract solution
		z3::expr_vector blocking_clause(ctx);

		sol.value = 0;
		for (size_t i = 0; i < n; i++) {
			z3::expr val = model.eval(x[i]);
			sol.x[i] = val.get_numeral_int();
			sol.value += sol.x[i];
			
			// Create blocking clause to find different solutions
			blocking_clause.push_back(x[i] != val);
		}
		
		solutions.push_back(sol);
		
		// Add blocking clause to prevent finding the same solution again
		solver.add(z3::mk_or(blocking_clause));
		count++;
	}
	
	// Sort by objective value
	std::sort(solutions.begin(), solutions.end(), [](const auto& a, const auto& b) {
			return a.value < b.value;
		});
	
	return solutions;
}
