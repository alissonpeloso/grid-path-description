# CSES Grid Paths - Backtracking Optimizations

An optimized backtracking solver and interactive visualizer for the **CSES 1625: Grid Paths** problem.

This project implements a depth-first search (DFS) with backtracking optimizations to find the number of paths in a **7x7 grid** starting at the top-left corner `(0, 0)` and ending at the bottom-left corner `(0, 6)`, visiting every cell exactly once in exactly 48 steps.

A naive backtracking search has an astronomical search space of $\approx 4^{48}$ states, which would take years to complete. By implementing targeted pruning heuristics, the search space is reduced to under **8.1 million DFS calls**, completing in **under 1 second**.

---

## 🛠️ Optimizations

### 1. Early Destination Check
* **Insight:** If the path reaches the destination cell `(0, 6)` before visiting all 49 cells, the search branch is immediately terminated.
* **Explanation:** Since a valid path must visit all cells exactly once, reaching the destination early means there will be unvisited cells left behind that can never be visited.
* **Code Snippet:**
  ```cpp
  if (x == 0 && y == n - 1) {
      return 0;
  }
  ```

### 2. Grid Splitting Pruning
* **Insight:** If the path touches a wall or a visited cell, cannot continue straight, but can turn in both perpendicular directions, the grid is split into two isolated, unvisited regions.
* **Explanation:** Since the path must be a continuous single line and cannot jump, it is topologically impossible to visit all cells in both isolated regions. We can prune the search branch immediately.
* **Code Snippet:**
  ```cpp
  // Path touches a vertical wall and can only turn Left or Right
  if (!canGoDown && !canGoUp && canGoLeft && canGoRight) {
      grid[y][x] = 0; // backtrack
      return 0;
  }
  // Path touches a horizontal wall and can only turn Up or Down
  if (canGoDown && canGoUp && !canGoLeft && !canGoRight) {
      grid[y][x] = 0; // backtrack
      return 0;
  }
  ```

### 3. Target Corner Neighbor Check
* **Insight:** When the path reaches the diagonal neighbor of the destination `(1, 5)`, and both adjacent entrances `(0, 5)` and `(1, 6)` are unvisited, prune the search.
* **Explanation:** Entering the destination `(0, 6)` from one neighbor blocks the path from ever visiting the other neighbor. Thus, one of the neighbors will remain unvisited.
* **Code Snippet:**
  ```cpp
  if (x == 1 && y == n - 2 && canGoLeft && canGoDown) {
      grid[y][x] = 0; // backtrack
      return 0;
  }
  ```

### 4. Diagonal Block Pruning
* **Insight:** If a diagonal neighbor cell (e.g., `(x+1, y+1)`) is blocked, and the path can turn in both perpendicular directions (e.g., Down and Right), prune the search.
* **Explanation:** The blocked diagonal cell acts as a pivot. Going in one direction traps the other orthogonal cell, creating an isolated region of unvisited cells.
* **Code Snippet:**
  ```cpp
  if (isBlocked(x+1, y+1) && canGoDown && canGoRight) { grid[y][x] = 0; return 0; }
  if (isBlocked(x-1, y+1) && canGoDown && canGoLeft)  { grid[y][x] = 0; return 0; }
  if (isBlocked(x+1, y-1) && canGoUp && canGoRight)   { grid[y][x] = 0; return 0; }
  if (isBlocked(x-1, y-1) && canGoUp && canGoLeft)    { grid[y][x] = 0; return 0; }
  ```

---

## 📈 Performance Comparison

| Optimization Level | DFS Call Count | Execution Time | Speedup Factor |
| :--- | :--- | :--- | :--- |
| **Base DFS** | $\approx 1.2 \times 10^{15}$ | Years | $1\times$ |
| **Opt 1 (Early End)** | $> 3,000,000,000$ | Timeout | Timeout |
| **Opt 2 (Grid Split)** | $46,816,433$ | $4.97\text{ s}$ | $62\times$ |
| **Opt 3 (Corner Check)** | $27,337,413$ | $3.11\text{ s}$ | $97\times$ |
| **Opt 4 (Diagonal Pivot)** | **$8,025,304$** | **$0.78\text{ s}$** | **$305\times$** |

---

## 💻 Compilation and Execution

To compile the C++ program, use a compiler supporting C++11 or higher (with `-O3` optimization flag for maximum performance):

```bash
# Compile
g++ -O3 paths_in_a_grid.cpp -o paths_in_a_grid

# Run
./paths_in_a_grid
```

### Input Format
The program reads a single string of 48 characters containing:
* `D` (Down)
* `U` (Up)
* `L` (Left)
* `R` (Right)
* `?` (Any direction)

For example:
```bash
??????R??????R??????????????????????????????????
```

---

## 🎨 Interactive Visualizer

The project includes `grid_paths_presentation.html`, a beautiful, interactive web-based visualizer built using TailwindCSS and FontAwesome. It allows you to:
- Run step-by-step or play/pause the backtracking animation.
- Inspect each optimization tab to see the mathematical and logical insights.
- View real-time search space call reduction metrics.
- Read corresponding C++ implementation snippets for each pruning technique.
