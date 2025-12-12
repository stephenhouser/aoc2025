#include <iostream>
#include <vector>
#include <algorithm>
#include <limits>
#include <numeric>

#include "minsolver.h"

#if 0
int main() {
    // Your example
    std::vector<std::vector<size_t>> A = {
        // {0, 0, 0, 0, 1, 1},  // x4 + x5 = 3
        // {0, 1, 0, 0, 0, 1},  // x1 + x5 = 4
        // {0, 0, 1, 1, 1, 0},  // x2 + x3 + x4 = 5
        // {1, 1, 0, 1, 0, 0}   // x0 + x1 + x3 = 7

		{1, 0, 1, 1, 0},
		{0, 0, 0, 1, 1},
		{1, 1, 0, 1, 1},
		{1, 1, 0, 0, 1},
		{1, 0, 1, 0, 1}
    };
    
    // std::vector<size_t> b = {3, 4, 5, 7};
    std::vector<size_t> b = {7, 5, 12, 7, 2};
        
    // Let's first check if there are solutions by exhaustive search
    std::cout << "=== Exhaustive Search for Verification ===" << std::endl;
    auto allSolutions = IntegerLinearSolver::findAllIntegerSolutions(A, b, 12);
    
    std::cout << "Found " << allSolutions.size() << " integer solutions:" << std::endl;
    for (size_t i = 0; i < std::min(allSolutions.size(), size_t(10)); i++) {
        int sum = std::accumulate(allSolutions[i].begin(), allSolutions[i].end(), 0);
        std::cout << "Solution " << i+1 << " (sum=" << sum << "): ";
        for (int val : allSolutions[i]) {
            std::cout << val << " ";
        }
        std::cout << std::endl;
    }
    std::cout << std::endl;
    
    // Now find optimal solution
    // std::cout << "=== Finding Optimal Solution ===" << std::endl;
    // auto solution = IntegerLinearSolver::solveMinSumInteger(A, b);
    // IntegerLinearSolver::printSolution(solution);
    
    // if (solution.hasSolution) {
    //     IntegerLinearSolver::verifySolution(A, b, solution.x);
    // }
    
    return 0;
}
#endif