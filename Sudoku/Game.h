#ifndef GAME_H
#define GAME_H
#include <SFML/Graphics.hpp>
#include "Board.h"

class Game {
public:
    Game();
    void run();
private:
    void processEvents();
    void update();
    void render();
    void drawGrid();
    void drawNumbers();
    void handlePlayerInput(sf::Keyboard::Key key);

    sf::RenderWindow window;
    Board board;
    sf::Font font;
    sf::Text text;

    int selectedRow;
    int selectedCol;
};

#endif // GAME_H