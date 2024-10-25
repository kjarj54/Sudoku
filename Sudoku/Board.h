#ifndef BOARD_H
#define BOARD_H
#include <array>

class Board {
public:
    Board();
    void generateNewPuzzle();
    bool solve();
    void display() const;
    bool isSafe(int row, int col, int num) const;
    bool findEmptyLocation(int &row, int &col) const;
    const std::array<std::array<int, 9>, 9>& getGrid() const;
    std::array<std::array<int, 9>, 9>& getGrid(); // Añadir esta línea
private:
    std::array<std::array<int, 9>, 9> grid;
};

#endif // BOARD_H