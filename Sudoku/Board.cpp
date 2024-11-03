#include "Board.h"
#include <cstdlib>
#include <ctime>

Board::Board() {
    // Inicializamos cada bloque en 0 usando CellBlock
    for (auto& blockRow : grid) {
        for (auto& block : blockRow) {
            for (int i = 0; i < 3; ++i) {
                for (int j = 0; j < 3; ++j) {
                    block.getCell(i, j) = 0;
                }
            }
        }
    }
}

void Board::generateNewPuzzle() {
    // Genera un tablero completo
    solve();

    // Marca todas las celdas como fijas en este punto
    for (int row = 0; row < 9; ++row) {
        for (int col = 0; col < 9; ++col) {
            isFixed[row][col] = true;
        }
    }

    // Elimina números para crear el puzzle
    int cellsToRemove = 40;
    while (cellsToRemove > 0) {
        int row = rand() % 9;
        int col = rand() % 9;

        int blockRow = row / 3;
        int blockCol = col / 3;
        int innerRow = row % 3;
        int innerCol = col % 3;

        if (grid[blockRow][blockCol].getCell(innerRow, innerCol) != 0) {
            grid[blockRow][blockCol].getCell(innerRow, innerCol) = 0;
            isFixed[row][col] = false; // Marca la celda como no fija
            cellsToRemove--;
        }
    }
}

bool Board::isCellFixed(int row, int col) const {
    return isFixed[row][col];
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
            grid[blockRow][blockCol].getCell(innerRow, innerCol) = num;
            if (solve()) {
                return true;
            }
            grid[blockRow][blockCol].getCell(innerRow, innerCol) = 0;
        }
    }
    return false;
}

void Board::display() const {
    for (int blockRow = 0; blockRow < 3; ++blockRow) {
        for (int innerRow = 0; innerRow < 3; ++innerRow) {
            for (int blockCol = 0; blockCol < 3; ++blockCol) {
                for (int innerCol = 0; innerCol < 3; ++innerCol) {
                    std::cout << grid[blockRow][blockCol].getCell(innerRow, innerCol) << " ";
                }
            }
            std::cout << std::endl;
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
        if (grid[row / 3][x / 3].getCell(row % 3, x % 3) == num ||
            grid[x / 3][col / 3].getCell(x % 3, col % 3) == num) {
            return false;
        }
    }

    // Verificar submatriz 3x3
    int startRow = row / 3;
    int startCol = col / 3;
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            if (grid[startRow][startCol].getCell(i, j) == num) {
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
                    if (grid[blockRow][blockCol].getCell(innerRow, innerCol) == 0) {
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

const std::array<std::array<CellBlock, 3>, 3>& Board::getGrid() const {
    return grid;
}

std::array<std::array<CellBlock, 3>, 3>& Board::getGrid() {
    return grid;
}
