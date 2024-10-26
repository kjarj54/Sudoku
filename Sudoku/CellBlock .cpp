#include "CellBlock.h"

CellBlock::CellBlock() {
    for (auto& row : cells) {
        row.fill(0);  // Inicializamos cada celda en 0
    }
}

int& CellBlock::getCell(int row, int col) {
    return cells[row][col];
}

const int& CellBlock::getCell(int row, int col) const {
    return cells[row][col];
}
