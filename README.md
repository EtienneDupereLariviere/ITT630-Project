# Sudoku Solver
The projects we created consists of two programs that solve some Sudoku. The first solution uses artificial intelligence to sequentially solve the grid and the second solution uses "pthreads" to allow parallel execution. There are several approaches to solve grids, those we have chosen to implement are resolutions inspired by human methods.

# Sequential Solution
For our sequential solution, we chose an algorithm CSP (Constraint satisfaction problem).

```
g++ -w -o sudoku SudokuSolver.cc cell.cc cell.h
/sudoku
```

# Parallel Solution
The program proceeds by elimination with the initial assumption that all values are possible for each box. When a value is set for a box, it is eliminated as a possibility for the other cells of the block, for the line and row that contain it.

```
sh makeLinux.sh
/sudoku
```
