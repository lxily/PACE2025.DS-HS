# Overview
This is a solver for the heuristic track of the PACE 2025 Challenge, targeting both the Dominating Set and Hitting Set problems. 
It supports both input and output formats as defined in the challenge for the two problems.

## Build

```shell
g++ -o solver -O2 -I . solver/lib/*.cpp solver/tools/*.cpp -std=c++2a
```

## Run
The solver reads input from standard input (stdin) and writes the output to standard output (stdout).

```shell
./solver < input_file > output_file
```