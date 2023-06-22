# Suguru Puzzle Solver

## Introduction

This repository includes code and data for solving Suguru puzzles, a pencil-and-paper puzzle that has been proven to be NP-complete, using SAT-based approach. 
The approach utilizing SAT encodes the constraints of the Suguru puzzle into a Boolean formula, which is subsequently tackled by a SAT solver (in this case, we use the MiniSAT SAT solver). 
We demonstrate that the encoding we have implemented is proportional to the size of the puzzle. 
More precisely, it is bounded by $O(m^3 n^3)$, where $m \times n$ represents the dimensions of the puzzle.

For more details about the Suguru puzzle, visit [Inabapuzzle](http://www.inabapuzzle.com/honkaku/nblock.html).

## Repository Components

Within this repository, you will find the following components:

* **Suguru Puzzle Solver**: Implemented in C++ using MiniSAT, the source code can be found in the `codes/solver.cpp` file.
* **Test Cases**: The repository provides a set of 180 test cases in plain text format for Suguru puzzles, obtained from [Janko](https://www.janko.at/Raetsel/Juosan/index.htm). These test cases are stored in the `test_cases` directory.
* **Experimental Data**: The repository includes experimental data that pertains to the runtime performance of the SAT-based approach solver. This data can be found in the `experimental_result` directory.

## How to Use

To use the Suguru puzzle solver and verifier, follow these straightforward steps:

1. For an extensive guide on how to compile the solver, please refer to this [medium blog](https://medium.com/@timbersama2020/minisat-installation-guide-efb99a897138). Another option is to directly utilize the pre-compiled executable file that is provided.
2. Execute the resulting executable file.
3. To use the solver, input the Suguru puzzle according to the format described in the [Input/Output Format](#inputoutput-format) section.

## Input/Output Format

The Suguru puzzle input is represented in the following format:

```
m n
h_{1,1} h_{1,2} ... h_{1,n}
h_{2,1} h_{2,2} ... h_{2,n}
...
h_{m,1} h_{m,2} ... h_{m,n}
r_{1,1} r_{1,2} ... r_{1,n}
r_{2,1} r_{2,2} ... r_{2,n}
...
r_{m,1} r_{m,2} ... r_{m,n}
```

In this format:
- `m` and `n` are integers that represent the number of rows and columns in the puzzle.
- `h_{i,j}` is an integer indicating the initial value of cell `(i,j)`, where `h_{i,j} = 0` represents an empty cell.
- `r_{i,j}` is an integer label indicating the region to which cell `(i,j)` belongs.

The Suguru solution, which is the output from the solver, is represented in the following format:

```
h_{1,1} h_{1,2} ... h_{1,n}
h_{2,1} h_{2,2} ... h_{2,n}
...
h_{m,1} h_{m,2} ... h_{m,n}
```

In this format, `h_{i,j}` is a positive integer, indicating the number contained in the cell `(i,j)`.
