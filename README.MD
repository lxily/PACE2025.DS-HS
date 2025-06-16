# The Heuristic Solver for the PACE 2025 Challenge

This is a solver (team Root) for the heuristic track of the PACE 2025 Challenge, targeting both the Dominating Set and Hitting Set problems. 
It supports both input and output formats as defined in the challenge for the two problems.

## How to Build and Run

### Build

```shell
g++ -o solver -O2 -I . solver/lib/*.cpp solver/tools/*.cpp -std=c++2a
```

### Run
The solver reads input from standard input (stdin) and writes the output to standard output (stdout).

```shell
./solver < input_file > output_file
```

**Note**: The solver uses the current timestamp as the random seed for each run, which may lead to slight variations in results. If multiple runs are allowed, evaluating the average or best performance across several runs can provide a more representative assessment of its effectiveness. 

## Brief Description of the Solver

### Problem Transformation

We unify the Dominating Set Problem and the Hitting Set Problem by transforming them into the unicost set covering problem.

- For the Dominating Set Problem, each vertex $v$ corresponds to an element $e_i$ and a set $s_i$ in the set covering problem. In the transformed problem, each element $e_i$ is covered by the sets corresponding to its neighboring vertices, and each set $s_i$ covers the elements corresponding to its neighbors.

- For the Hitting Set Problem, each initial set $s_i^o$ corresponds to an element $e_i$, and each element $e_i^o$ in $s_i^o$ corresponds to a set $s_i$ in the set covering problem. In the transformed problem, each element is covered by the sets in which it appears, and each set naturally covers all the elements it contains.

Thus, the objectives of both original problems are unified as: Minimize the number of selected sets that cover all elements.

### Preprocessing via Reduction Rules

We employ several classical reduction rules to shrink the problem size while preserving optimality:

- *Element dominance*:  
   If the set of sets covering $e_i$ is a subset of those covering $e_j$ (where $i \ne j$), then element $e_j$ can be safely removed.
- *Set dominance*:  
   If the set of elements covered by $s_i$ is a subset of those covered by $s_j$, then set $s_i$ can be removed.
- *Mandatory coverage*:  
   If an element $e_i$ is only covered by one set $s_i$, then $s_i$ must be selected, and all the elements it covers can be removed.

Iterative application of these rules often leads to substantial simplification of the instance before optimization begins.

### Frequency-Based Initial Construction

To generate high-quality starting solutions, we adopt a multi-round greedy heuristic guided by element frequencies. 
In each round, we prioritize sets that cover more critical (i.e., less frequently covered) elements. We further refine the importance scores of elements based on how many times they are actually covered in current solutions. 
This iterative process improves the coverage quality with acceptable construction time overhead.

### Local Search with Adaptive Weighting

Starting from a feasible initial cover, we iteratively remove a randomly selected set and attempt to reestablish a feasible cover without increasing the number of sets used. Once such a reconstruction is successful, we obtain an improved solution.  

Weighting techniques have demonstrated strong effectiveness in various set covering–related problems¹ ² ³. Our solver adopts a similar strategy: we assign adaptive weights to currently uncovered elements, and seek to minimize the total weight of uncovered elements during local search.  

The neighborhood is defined by a pairwise swap operation: removing one set from the solution and adding another.

In each iteration:
1. A random uncovered element is selected.
2. For this element, we try to add one of the sets that can cover it.
3. For each such added set, we try removing one set currently in the solution.
4. We evaluate all swap pairs and select the one that minimizes the total weight of uncovered elements after the move.

When the search reaches a local optimum, we increase the weight of a random uncovered element, encouraging its coverage in future iterations. 
Once a new feasible solution is obtained, we proactively remove a random set to trigger the next search phase with one fewer set. 
The solver terminates when the maximum time limit is reached, and returns the best solution found so far.

### References

¹ *Zhang Q, Lü Z, Su Z, et al. Vertex weighting-based tabu search for p-center problem[C]//Proceedings of the Twenty-Ninth International Conference on International Joint Conferences on Artificial Intelligence. 2021: 1481-1487.*  
² *Su Z, Zhang Q, Lü Z, et al. Weighting-based variable neighborhood search for optimal camera placement[C]//Proceedings of the AAAI Conference on Artificial Intelligence. 2021, 35(14): 12400-12408.*  
³ *Zhang Q, Lü Z, Su Z, et al. A vertex weighting-based double-tabu search algorithm for the classical p-center problem[J]. Computers & Operations Research, 2023, 160: 106373.*

## Contact

For any questions or feedback, please contact:

**Canhui Luo**  
📧 [luocanhui@hust.edu.cn](mailto:luocanhui@hust.edu.cn) / [im.lch@foxmail.com](mailto:im.lch@foxmail.com)

## License

This project is licensed under the [MIT License](LICENSE). 