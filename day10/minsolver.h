class IntegerLinearSolver {
private:
    static const int MAX_VALUE = 50; // Upper bound for search
    
public:
    struct Solution {
        std::vector<int> x = {};
        int objectiveValue = std::numeric_limits<int>::max();;
        bool hasSolution = false;
        int solutionsExplored = 0;
        std::string message = "";
    };
    
    static Solution solveMinSumInteger(const std::vector<std::vector<size_t>>& A, 
                                      const std::vector<size_t>& b) {
        Solution result;
        // result.hasSolution = false;
        // result.objectiveValue = std::numeric_limits<int>::max();
        // result.solutionsExplored = 0;
        
        size_t m = A.size();    // number of equations
        size_t n = A[0].size(); // number of variables
        
        if (m == 0 || n == 0) {
            result.message = "Empty matrix";
            return result;
        }
        
        std::vector<int> currentSolution(n, 0);
        std::vector<int> bestSolution(n, 0);
        int bestSum = std::numeric_limits<int>::max();
        bool foundSolution = false;
        
        // Start backtracking from variable 0
        backtrack(A, b, currentSolution, 0, bestSolution, bestSum, foundSolution, result.solutionsExplored);
        
        if (foundSolution) {
            result.x = bestSolution;
            result.objectiveValue = bestSum;
            result.hasSolution = true;
            result.message = "Optimal integer solution found";
        } else {
            result.message = "No integer solution found within search bounds";
        }
        
        return result;
    }
    
private:
    static void backtrack(const std::vector<std::vector<size_t>>& A,
                         const std::vector<size_t>& b,
                         std::vector<int>& currentSolution,
                         size_t variableIndex,
                         std::vector<int>& bestSolution,
                         int& bestSum,
                         bool& foundSolution,
                         int& solutionsExplored) {
        
        size_t n = currentSolution.size();
        
        // If we've assigned all variables, check if it's a valid solution
        if (variableIndex == n) {
            if (isValidSolution(A, b, currentSolution)) {
                int currentSum = std::accumulate(currentSolution.begin(), currentSolution.end(), 0);
                solutionsExplored++;
                
                if (currentSum < bestSum) {
                    bestSum = currentSum;
                    bestSolution = currentSolution;
                    foundSolution = true;
                    
                    // Print intermediate solutions
                    std::cout << "Found solution with sum " << currentSum << ": ";
                    for (int val : currentSolution) {
                        std::cout << val << " ";
                    }
                    std::cout << std::endl;
                }
            }
            return;
        }
        
        // Try values for current variable
        for (int value = 0; value <= MAX_VALUE; value++) {
            currentSolution[variableIndex] = value;
            
            // Pruning: if current partial sum already exceeds best, skip
            int partialSum = 0;
            for (size_t i = 0; i <= variableIndex; i++) {
                partialSum += currentSolution[i];
            }
            
            if (partialSum >= bestSum) {
                continue; // Prune this branch
            }
            
            // Continue without early feasibility check that might be too restrictive
            backtrack(A, b, currentSolution, variableIndex + 1, bestSolution, bestSum, foundSolution, solutionsExplored);
        }
    }
    
    static bool isValidSolution(const std::vector<std::vector<size_t>>& A,
                               const std::vector<size_t>& b,
                               const std::vector<int>& x) {
        size_t m = A.size();
        size_t n = A[0].size();
        
        for (size_t i = 0; i < m; i++) {
            long long sum = 0;  // Use long long to avoid overflow
            for (size_t j = 0; j < n; j++) {
                sum += static_cast<long long>(A[i][j]) * x[j];
            }
            if (sum != static_cast<long long>(b[i])) {
                return false;
            }
        }
        return true;
    }
    
public:
    static void printSolution(const Solution& sol) {
        std::cout << "Status: " << sol.message << std::endl;
        std::cout << "Solutions explored: " << sol.solutionsExplored << std::endl;
        
        if (sol.hasSolution) {
            std::cout << "Minimum sum of variables: " << sol.objectiveValue << std::endl;
            std::cout << "Optimal solution vector x:" << std::endl;
            for (size_t i = 0; i < sol.x.size(); i++) {
                std::cout << "x[" << i << "] = " << sol.x[i] << std::endl;
            }
        }
        std::cout << std::endl;
    }
    
    static bool verifySolution(const std::vector<std::vector<size_t>>& A, 
                              const std::vector<size_t>& b, 
                              const std::vector<int>& x) {
        size_t m = A.size();
        size_t n = A[0].size();
        
        std::cout << "Verification:" << std::endl;
        bool isValid = true;
        
        // Check constraints Ax = b
        for (size_t i = 0; i < m; i++) {
            long long sum = 0;
            for (size_t j = 0; j < n; j++) {
                sum += static_cast<long long>(A[i][j]) * x[j];
            }
            
            std::cout << "Equation " << i << ": " << sum << " = " << b[i];
            
            if (sum != static_cast<long long>(b[i])) {
                std::cout << " ❌";
                isValid = false;
            } else {
                std::cout << " ✓";
            }
            std::cout << std::endl;
        }
        
        // Check non-negativity
        std::cout << "Non-negativity constraints:" << std::endl;
        for (size_t j = 0; j < n; j++) {
            std::cout << "x[" << j << "] = " << x[j];
            if (x[j] < 0) {
                std::cout << " ❌ (negative)";
                isValid = false;
            } else {
                std::cout << " ✓";
            }
            std::cout << std::endl;
        }
        
        // Calculate sum
        int sum = std::accumulate(x.begin(), x.end(), 0);
        std::cout << "Sum of variables: " << sum << std::endl;
        
        return isValid;
    }
    
    // Exhaustive search method for verification
    static std::vector<std::vector<int>> findAllIntegerSolutions(
        const std::vector<std::vector<size_t>>& A, 
        const std::vector<size_t>& b,
        int maxValue = 15) {
        
        std::vector<std::vector<int>> solutions;
        size_t n = A[0].size();
        std::vector<int> currentSolution(n, 0);
        
        std::cout << "Searching for all solutions with variables <= " << maxValue << "..." << std::endl;
        
        findAllSolutionsRecursive(A, b, currentSolution, 0, maxValue, solutions);
        
        // Sort by sum
        std::sort(solutions.begin(), solutions.end(), 
                 [](const std::vector<int>& a, const std::vector<int>& b) {
                     int sumA = std::accumulate(a.begin(), a.end(), 0);
                     int sumB = std::accumulate(b.begin(), b.end(), 0);
                     return sumA < sumB;
                 });
        
        return solutions;
    }
    
private:
    static void findAllSolutionsRecursive(const std::vector<std::vector<size_t>>& A,
                                         const std::vector<size_t>& b,
                                         std::vector<int>& currentSolution,
                                         size_t variableIndex,
                                         int maxValue,
                                         std::vector<std::vector<int>>& solutions) {
        size_t n = currentSolution.size();
        
        if (variableIndex == n) {
            if (isValidSolution(A, b, currentSolution)) {
                solutions.push_back(currentSolution);
            }
            return;
        }
        
        for (int value = 0; value <= maxValue; value++) {
            currentSolution[variableIndex] = value;
            findAllSolutionsRecursive(A, b, currentSolution, variableIndex + 1, maxValue, solutions);
        }
    }
};
