#ifndef BOARD_H
#define BOARD_H

#include <array>
#include <iostream>
#include "CellBlock.h"

class Board {
public:
    Board();

    void generateNewPuzzle();
    bool solve();
    void display() const;
    bool isSafe(int row, int col, int num) const;
    bool findEmptyLocation(int& row, int& col) const;

    const array<array<CellBlock, 3>, 3>& getGrid() const;
    array<array<CellBlock, 3>, 3>& getGrid();

private:
    array<array<CellBlock, 3>, 3> grid;
};

#endif // BOARD_H
