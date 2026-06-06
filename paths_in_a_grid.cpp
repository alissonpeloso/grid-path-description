#include <bits/stdc++.h>

using namespace std;

int n = 7;
vector<vector<int> > grid(n, vector<int>(n, 0));
int callCount = 0;

bool isBlocked(int x, int y) {
    return x < 0 || x >= n || y < 0 || y >= n || grid[y][x];
}

int countPaths(int x, int y, int count, string pathDescription) {
    callCount++;

    // If count = n * n, we've visited all cells
    if (count == n * n && x == 0 && y == n - 1) {
        return 1;
    }

    // Optimization 1: Mirror could not be applied here because the destination is at left-down corner.

    // Optimization 2: If it has visited the destination before get all filled out, we can stop.
    if (x == 0 && y == n - 1) {
        return 0;
    }

    grid[y][x] = 1;

    // We have 4 options, directions
    int down = 0, up = 0, left = 0, right = 0;
    char currentInstruction = pathDescription[0];

    bool canGoDown = !isBlocked(x, y+1) && (currentInstruction == 'D' || currentInstruction == '?');
    bool canGoUp = !isBlocked(x, y-1) && (currentInstruction == 'U' || currentInstruction == '?');
    bool canGoLeft = !isBlocked(x-1, y) && (currentInstruction == 'L' || currentInstruction == '?');
    bool canGoRight = !isBlocked(x+1, y) && (currentInstruction == 'R' || currentInstruction == '?');

    // Optimization 3: If the path touches a wall and can turn either left or right, the grid splits into two parts
    // that contain unvisited squares, then we cannot visit all squares anymore.
    if (!canGoDown && !canGoUp && canGoLeft && canGoRight) {
        // backtracking
        grid[y][x] = 0;

        return 0;
    }

    // Optimization 4: Same logic as Optimization 3 but the other way around, if the path touches a wall and can turn
    // either up or down, the grid splits into two parts
    if (canGoDown && canGoUp && !canGoLeft && !canGoRight) {
        // backtracking
        grid[y][x] = 0;

        return 0;
    }

    // Optimization 5: when into the first diagonal of destination and it's possible to go down and left, we can stop
    // because we cannot go back up and right anymore, so we cannot visit all squares.
    // Call count: 46816433 -> 32779359
    if (x == 1 && y == n - 2 && canGoLeft && canGoDown) {
        // backtracking
        grid[y][x] = 0;

        return 0;
    }

    if (x == n - 2 && y == n - 2 && !canGoUp && !grid[y - 1][x + 1]) {
        // backtracking
        grid[y][x] = 0;

        return 0;
    }

    if (x == 1 && y == 1 && canGoUp) {
        canGoDown = false;
        canGoLeft = false;
    }

    if (x == 1 && y == 1 && canGoLeft) {
        canGoDown = false;
        canGoRight = false;
    }

    /**
     * Diagonal cell blocked
     */

    // Down-right diagonal
    if (isBlocked(x+1, y+1) && canGoDown && canGoRight) {
        // back-tracking
        grid[y][x] = 0;

        return 0;
    }

    // Down-left diagonal
    if (isBlocked(x-1, y+1) && canGoDown && canGoLeft) {
        // back-tracking
        grid[y][x] = 0;

        return 0;
    }

    // Up-right diagonal
    if (isBlocked(x+1, y-1) && canGoUp && canGoRight) {
        // back-tracking
        grid[y][x] = 0;

        return 0;
    }

    // Up-left diagonal
    if (isBlocked(x-1, y-1) && canGoUp && canGoLeft) {
        // back-tracking
        grid[y][x] = 0;

        return 0;
    }

    // Down
    if (canGoDown) {
        down = countPaths(x, y + 1, count + 1, pathDescription.substr(1));
    }

    // Up
    if (canGoUp) {
        up = countPaths(x, y - 1, count + 1, pathDescription.substr(1));
    }

    // Left
    if (canGoLeft) {
        left = countPaths(x - 1, y, count + 1, pathDescription.substr(1));
    }

    // Right
    if (canGoRight) {
        right = countPaths(x + 1, y, count + 1, pathDescription.substr(1));
    }

    // back-tracking
    grid[y][x] = 0;

    return down + up + left + right;
}

int main() {
    string pathDescription;
    cin >> pathDescription;

    cout << countPaths(0, 0, 1, pathDescription) << '\n';
    cout << callCount << '\n';

    return 0;
}
