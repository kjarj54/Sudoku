#include "Game.h"
#include <iostream>

Game::Game() : window(sf::VideoMode(640, 600), "Sudoku Game"), selectedRow(0), selectedCol(0) {
    if (!font.loadFromFile("arial.ttf")) {
        std::cerr << "Error loading font 'arial.ttf'\n";
    }
    text.setFont(font);
    text.setCharacterSize(24);
    text.setFillColor(sf::Color::Black);

    showMenu = true; // Indicamos que iniciamos en la pantalla de menú
    initializeButtons();
}

void Game::initializeButtons() {
    // Botón para Nuevo Juego
    newGameButton.setSize(sf::Vector2f(200, 50));
    newGameButton.setFillColor(sf::Color::Blue);
    newGameButton.setPosition(220, 200);

    newGameText.setFont(font);
    newGameText.setString("Nuevo Juego");
    newGameText.setCharacterSize(24);
    newGameText.setPosition(250, 210);

    // Botón para Cargar Juego
    loadGameButton.setSize(sf::Vector2f(200, 50));
    loadGameButton.setFillColor(sf::Color::Green);
    loadGameButton.setPosition(220, 300);

    loadGameText.setFont(font);
    loadGameText.setString("Cargar Sudoku");
    loadGameText.setCharacterSize(24);
    loadGameText.setPosition(240, 310);

    // Botón para resolver
    solveButton.setSize(sf::Vector2f(100, 30));
    solveButton.setFillColor(sf::Color::Red);
    solveButton.setPosition(500, 20);

    solveText.setFont(font);
    solveText.setString("Solve");
    solveText.setCharacterSize(20);
    solveText.setPosition(510, 25);
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
        }
        else if (event.type == sf::Event::MouseButtonPressed) {
            handleMouseClick(event.mouseButton.x, event.mouseButton.y);
        }
        else if (!showMenu && event.type == sf::Event::KeyPressed) {
            handlePlayerInput(event.key.code);
        }
    }
}


// Manejador de clics
void Game::handleMouseClick(int x, int y) {
    if (showMenu) {
        if (newGameButton.getGlobalBounds().contains(x, y)) {
            board.generateNewPuzzle();
            showMenu = false;
        }
        else if (loadGameButton.getGlobalBounds().contains(x, y)) {
            // Aquí agregarías la lógica para cargar un Sudoku desde un archivo
            showMenu = false;
        }
    }
    else if (solveButton.getGlobalBounds().contains(x, y)) {
        board.solve();
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

        // Asigna el número a la celda específica usando la estructura CellBlock
        board.getGrid()[blockRow][blockCol].getCell(innerRow, innerCol) = num;
    }
}

void Game::update() {
    // Aquí puedes agregar lógica adicional para actualizar el estado del juego
}

void Game::render() {
    window.clear(sf::Color::White);

    if (showMenu) {
        window.draw(newGameButton);
        window.draw(newGameText);
        window.draw(loadGameButton);
        window.draw(loadGameText);
    }
    else {
        drawGrid();
        drawNumbers();
        window.draw(solveButton);
        window.draw(solveText);
    }

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
                    int num = grid[i][j].getCell(bi, bj);  // Acceso mediante getCell
                    if (num != 0) {
                        text.setString(std::to_string(num));
                        text.setPosition(50 + (j * 3 + bj) * 60 + 20, 50 + (i * 3 + bi) * 60 + 10);
                        window.draw(text);
                    }
                }
            }
        }
    }

    // Resalta la celda seleccionada
    sf::RectangleShape highlight(sf::Vector2f(60, 60));
    highlight.setFillColor(sf::Color(0, 0, 255, 50)); // Azul semi-transparente
    highlight.setPosition(50 + selectedCol * 60, 50 + selectedRow * 60);
    window.draw(highlight);
}
