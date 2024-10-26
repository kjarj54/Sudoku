#include "Board.h"
#include <iostream>
#include <cstdlib>
#include <ctime>

Board::Board() {
    for (auto& blockRow : grid) {
        for (auto& block : blockRow) {
            block.fill({ 0 });
        }
    }
}

void Board::generateNewPuzzle() {
    srand(time(0));
    for (int blockRow = 0; blockRow < 3; ++blockRow) {
        for (int blockCol = 0; blockCol < 3; ++blockCol) {
            int num = (rand() % 9) + 1;
            int row = rand() % 3;
            int col = rand() % 3;
            if (isSafe(blockRow * 3 + row, blockCol * 3 + col, num)) {
                grid[blockRow][blockCol][row][col] = num;
            }
        }
    }
}

bool Board::solve() {
    int row, col;
    if (!findEmptyLocation(row, col)) {
        return true; // Puzzle solved
    }

    int blockRow = row / 3;
    int blockCol = col / 3;
    int innerRow = row % 3;
    int innerCol = col % 3;

    for (int num = 1; num <= 9; num++) {
        if (isSafe(row, col, num)) {
            grid[blockRow][blockCol][innerRow][innerCol] = num;
            if (solve()) {
                return true;
            }
            grid[blockRow][blockCol][innerRow][innerCol] = 0;
        }
    }
    return false;
}

void Board::display() const {
    for (int blockRow = 0; blockRow < 3; ++blockRow) {
        for (int innerRow = 0; innerRow < 3; ++innerRow) {
            for (int blockCol = 0; blockCol < 3; ++blockCol) {
                for (int innerCol = 0; innerCol < 3; ++innerCol) {
                    cout << grid[blockRow][blockCol][innerRow][innerCol] << " ";
                }
            }
            cout << endl;
        }
    }
}

bool Board::isSafe(int row, int col, int num) const {
    int blockRow = row / 3;
    int blockCol = col / 3;
    int innerRow = row % 3;
    int innerCol = col % 3;

    // Verificar fila y columna
    for (int x = 0; x < 9; ++x) {
        if (grid[row / 3][x / 3][row % 3][x % 3] == num ||
            grid[x / 3][col / 3][x % 3][col % 3] == num) {
            return false;
        }
    }

    // Verificar submatriz 3x3
    int startRow = row / 3;
    int startCol = col / 3;
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            if (grid[startRow][startCol][i][j] == num) {
                return false;
            }
        }
    }

    return true;
}

bool Board::findEmptyLocation(int& row, int& col) const {
    for (int blockRow = 0; blockRow < 3; ++blockRow) {
        for (int blockCol = 0; blockCol < 3; ++blockCol) {
            for (int innerRow = 0; innerRow < 3; ++innerRow) {
                for (int innerCol = 0; innerCol < 3; ++innerCol) {
                    if (grid[blockRow][blockCol][innerRow][innerCol] == 0) {
                        row = blockRow * 3 + innerRow;
                        col = blockCol * 3 + innerCol;
                        return true;
                    }
                }
            }
        }
    }
    return false;
}

const array<array<array<array<int, 3>, 3>, 3>, 3>& Board::getGrid() const {
    return grid;
}

array<array<array<array<int, 3>, 3>, 3>, 3>& Board::getGrid() {
    return grid;
}