#include "Board.h"
#include <iostream>
#include <cstdlib>
#include <ctime>

Board::Board() {
    // Initialize the grid with zeros
    grid.fill({ 0 });
}

void Board::generateNewPuzzle() {
    // Simple puzzle generation logic for demonstration
    srand(time(0));
    for (int i = 0; i < 9; ++i) {
        int num = (rand() % 9) + 1;
        if (isSafe(i, i, num)) {
            grid[i][i] = num;
        }
    }
}

bool Board::solve() {
    int row, col;
    if (!findEmptyLocation(row, col)) {
        return true; // Puzzle solved
    }

    for (int num = 1; num <= 9; num++) {
        if (isSafe(row, col, num)) {
            grid[row][col] = num;
            if (solve()) {
                return true;
            }
            grid[row][col] = 0;
        }
    }
    return false;
}

void Board::display() const {
    for (const auto &row : grid) {
        for (int num : row) {
            cout << num << " ";
        }
        cout << endl;
    }
}

bool Board::isSafe(int row, int col, int num) const {
    // Check row
    for (int x = 0; x < 9; x++) {
        if (grid[row][x] == num) {
            return false;
        }
    }

    // Check column
    for (int x = 0; x < 9; x++) {
        if (grid[x][col] == num) {
            return false;
        }
    }

    // Check 3x3 subgrid
    int startRow = row - row % 3;
    int startCol = col - col % 3;
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            if (grid[i + startRow][j + startCol] == num) {
                return false;
            }
        }
    }

    return true;
}

bool Board::findEmptyLocation(int &row, int &col) const {
    for (row = 0; row < 9; row++) {
        for (col = 0; col < 9; col++) {
            if (grid[row][col] == 0) {
                return true;
            }
        }
    }
    return false;
}
const array<array<int, 9>, 9>& Board::getGrid() const {
    return grid;
}

array<array<int, 9>, 9>& Board::getGrid() {
    return grid;
}