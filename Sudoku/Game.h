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
    void initializeButtons();
    void handleMouseClick(int x, int y);
    void saveGame();
    void loadGame();


    sf::RenderWindow window;
    sf::Font font;
    sf::Text text;

    Board board;
    int selectedRow;
    int selectedCol;

    bool showMenu;
    sf::RectangleShape newGameButton;
    sf::Text newGameText;
    sf::RectangleShape loadGameButton;
    sf::Text loadGameText;
    sf::RectangleShape saveGameButton; // Botón de guardar
    sf::Text saveGameText;
    sf::RectangleShape solveButton;
    sf::Text solveText;
};

#endif // GAME_H
