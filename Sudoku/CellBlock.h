#ifndef CELLBLOCK_H
#define CELLBLOCK_H

#include <array>
#include <iostream>

using namespace std;

class CellBlock {
public:
    CellBlock();

    int& getCell(int row, int col);
    const int& getCell(int row, int col) const;

private:
    array<array<int, 3>, 3> cells;
};

#endif // CELLBLOCK_H