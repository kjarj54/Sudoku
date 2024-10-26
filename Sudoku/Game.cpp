#include "Game.h"

Game::Game() : window(sf::VideoMode(640, 600), "Sudoku Game"), selectedRow(0), selectedCol(0) {
    board.generateNewPuzzle();
    if (!font.loadFromFile("arial.ttf")) {
        // Handle error
    }
    text.setFont(font);
    text.setCharacterSize(24);
    text.setFillColor(sf::Color::Black);
}

void Game::run() {
    while (window.isOpen()) {
        processEvents();
        update();
        render();
    }
}

void Game::processEvents() {
    sf::Event event;
    while (window.pollEvent(event)) {
        if (event.type == sf::Event::Closed) {
            window.close();
        } else if (event.type == sf::Event::KeyPressed) {
            handlePlayerInput(event.key.code);
        }
    }
}

void Game::handlePlayerInput(sf::Keyboard::Key key) {
    if (key == sf::Keyboard::Up) {
        selectedRow = (selectedRow + 8) % 9;
    }
    else if (key == sf::Keyboard::Down) {
        selectedRow = (selectedRow + 1) % 9;
    }
    else if (key == sf::Keyboard::Left) {
        selectedCol = (selectedCol + 8) % 9;
    }
    else if (key == sf::Keyboard::Right) {
        selectedCol = (selectedCol + 1) % 9;
    }
    else if (key >= sf::Keyboard::Num1 && key <= sf::Keyboard::Num9) {
        int num = key - sf::Keyboard::Num1 + 1;

        // Convert selectedRow and selectedCol to the appropriate block and inner indices
        int blockRow = selectedRow / 3;
        int blockCol = selectedCol / 3;
        int innerRow = selectedRow % 3;
        int innerCol = selectedCol % 3;

        // Assign the number to the specific cell in the grid
        board.getGrid()[blockRow][blockCol][innerRow][innerCol] = num;
    }
}

void Game::update() {
    // Aquí puedes agregar lógica adicional para actualizar el estado del juego
}

void Game::render() {
    window.clear(sf::Color::White);
    drawGrid();
    drawNumbers();
    window.display();
}

void Game::drawGrid() {
    sf::RectangleShape line(sf::Vector2f(540, 2));
    line.setFillColor(sf::Color::Black);
    for (int i = 0; i <= 9; ++i) {
        line.setPosition(50, 50 + i * 60);
        window.draw(line);
        line.setPosition(50 + i * 60, 50);
        line.setRotation(90);
        window.draw(line);
        line.setRotation(0);
    }
}

void Game::drawNumbers() {
    const auto& grid = board.getGrid();
    for (int i = 0; i < 3; ++i) {
        for (int j = 0; j < 3; ++j) {
            for (int bi = 0; bi < 3; ++bi) {
                for (int bj = 0; bj < 3; ++bj) {
                    int num = grid[i][j][bi][bj];
                    if (num != 0) {
                        text.setString(std::to_string(num));
                        text.setPosition(50 + (j * 3 + bj) * 60 + 20, 50 + (i * 3 + bi) * 60 + 10);
                        window.draw(text);
                    }
                }
            }
        }
    }

    // Highlight the selected cell
    sf::RectangleShape highlight(sf::Vector2f(60, 60));
    highlight.setFillColor(sf::Color(0, 0, 255, 50)); // Semi-transparent blue
    highlight.setPosition(50 + selectedCol * 60, 50 + selectedRow * 60);
    window.draw(highlight);
}