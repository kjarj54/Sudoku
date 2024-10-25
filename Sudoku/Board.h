#ifndef BOARD_H
#define BOARD_H
#include <array>
#include <iostream> 

using namespace std;

class Board {
public:
    Board();
    void generateNewPuzzle();
    bool solve();
    void display() const;
    bool isSafe(int row, int col, int num) const;
    bool findEmptyLocation(int &row, int &col) const;
    const array<array<int, 9>, 9>& getGrid() const;
    array<array<int, 9>, 9>& getGrid(); // Añadir esta línea
private:
    array<array<int, 9>, 9> grid;
};

#endif // BOARD_H