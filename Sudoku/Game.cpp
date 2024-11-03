#include "Game.h"
#include <iostream>
#include <fstream>

using namespace std;

Game::Game() : window(sf::VideoMode(640, 600), "Sudoku Game"), selectedRow(0), selectedCol(0) {
    if (!font.loadFromFile("arial.ttf")) {
        cerr << "Error loading font 'arial.ttf'\n";
    }
    text.setFont(font);
    text.setCharacterSize(24);
    text.setFillColor(sf::Color::Black);

    showMenu = true; // Indicamos que iniciamos en la pantalla de menú
    initializeButtons();
}

void Game::saveGame() {
    std::ofstream outFile("sudoku_save.txt");
    if (!outFile) {
        std::cerr << "Error al abrir el archivo para guardar el juego.\n";
        return;
    }
    std::cout << "Guardando juego...\n"; // Mensaje de depuración
    const auto& grid = board.getGrid();
    for (int i = 0; i < 3; ++i) {
        for (int j = 0; j < 3; ++j) {
            for (int bi = 0; bi < 3; ++bi) {
                for (int bj = 0; bj < 3; ++bj) {
                    outFile << grid[i][j].getCell(bi, bj) << " ";
                }
            }
            outFile << "\n";
        }
    }
    outFile.close();
    std::cout << "Juego guardado exitosamente.\n"; // Confirmación de éxito
}

void Game::loadGame() {
    std::ifstream inFile("sudoku_save.txt");
    if (!inFile) {
        std::cerr << "Error al abrir el archivo para cargar el juego.\n";
        return;
    }
    auto& grid = board.getGrid();
    for (int i = 0; i < 3; ++i) {
        for (int j = 0; j < 3; ++j) {
            for (int bi = 0; bi < 3; ++bi) {
                for (int bj = 0; bj < 3; ++bj) {
                    inFile >> grid[i][j].getCell(bi, bj);
                }
            }
        }
    }
    inFile.close();
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

    // Botón para Guardar Juego
    saveGameButton.setSize(sf::Vector2f(100, 30));
    saveGameButton.setFillColor(sf::Color::Magenta);
    saveGameButton.setPosition(350, 20);
    saveGameText.setFont(font);
    saveGameText.setString("Guardar");
    saveGameText.setCharacterSize(20);
    saveGameText.setPosition(360, 25);

    // Botón para Resolver
    solveButton.setSize(sf::Vector2f(100, 30));
    solveButton.setFillColor(sf::Color::Red);
    solveButton.setPosition(500, 20);
    solveText.setFont(font);
    solveText.setString("Solve");
    solveText.setCharacterSize(20);
    solveText.setPosition(510, 25);

    // Botón de Pista
    hintButton.setSize(sf::Vector2f(100, 30));
    hintButton.setFillColor(sf::Color::Green);
    hintButton.setPosition(230, 20); 
    hintText.setFont(font);
    hintText.setString("Pista");
    hintText.setCharacterSize(20);
    hintText.setPosition(240, 25);
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
            gameCompleted = false;
        }
        else if (loadGameButton.getGlobalBounds().contains(x, y)) {
            loadGame();
            showMenu = false;
            gameCompleted = false;
        }
    }
    else if (solveButton.getGlobalBounds().contains(x, y)) {
        board.solve();
        displayMessage("Sudoku completado con ayuda de la máquina.");
    }
    else if (saveGameButton.getGlobalBounds().contains(x, y)) {
        saveGame();
    }
    else if (hintButton.getGlobalBounds().contains(x, y)) {
        giveHint(); 
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

        // Determina la celda en términos de subcuadro
        int blockRow = selectedRow / 3;
        int blockCol = selectedCol / 3;
        int innerRow = selectedRow % 3;
        int innerCol = selectedCol % 3;

        // Verifica si la celda es fija y evita cambios
        if (board.isCellFixed(selectedRow, selectedCol)) {
            std::cout << "No se puede modificar una celda fija.\n";
            return;
        }

        // Verifica si el número ya está en la celda seleccionada
        if (board.getGrid()[blockRow][blockCol].getCell(innerRow, innerCol) == num) {
            std::cout << "El número ya está en la celda.\n";
            return;
        }

        // Usa isSafe para verificar las reglas de Sudoku
        if (board.isSafe(selectedRow, selectedCol, num)) {
            // Si es seguro, coloca el número en la celda específica
            board.getGrid()[blockRow][blockCol].getCell(innerRow, innerCol) = num;
        }
        else {
            std::cout << "Movimiento inválido según las reglas de Sudoku.\n";
        }
    }
}

void Game::giveHint() {
    int row, col;
    if (!board.findEmptyLocation(row, col)) {
        std::cout << "No hay celdas vacías para dar una pista.\n";
        return;
    }

    // Guarda el estado original
    auto& grid = board.getGrid();
    std::array<std::array<CellBlock, 3>, 3> originalGrid = grid;

    // Resuelve el tablero temporalmente para obtener el valor correcto en (row, col)
    if (board.solve()) {
        int blockRow = row / 3;
        int blockCol = col / 3;
        int innerRow = row % 3;
        int innerCol = col % 3;
        int hint = grid[blockRow][blockCol].getCell(innerRow, innerCol);

        // Restaura el estado original
        grid = originalGrid;

        // Coloca la pista en la celda seleccionada
        board.getGrid()[blockRow][blockCol].getCell(innerRow, innerCol) = hint;
        std::cout << "Pista: La celda en (" << row << ", " << col << ") debería ser " << hint << ".\n";
    }
    else {
        std::cout << "No se puede dar una pista en este momento.\n";
    }
}

void Game::update() {
    if (!gameCompleted) {
        bool isComplete = true;
        for (int row = 0; row < 9; ++row) {
            for (int col = 0; col < 9; ++col) {
                int blockRow = row / 3;
                int blockCol = col / 3;
                int innerRow = row % 3;
                int innerCol = col % 3;
                int num = board.getGrid()[blockRow][blockCol].getCell(innerRow, innerCol);

                // Verifica si alguna celda está vacía o si el número en ella no es válido
                if (num == 0 || !board.isSafe(row, col, num)) {
                    isComplete = false;
                    break;
                }
            }
            if (!isComplete) break;
        }

        // Si el tablero está completo y válido
        if (isComplete) {
            gameCompleted = true;
            displayMessage("¡Felicidades! Completaste el Sudoku correctamente.");
        }
    }
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
        window.draw(saveGameButton);
        window.draw(saveGameText);
        window.draw(hintButton);
        window.draw(hintText);
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

void Game::displayMessage(const std::string& message) {
    sf::RenderWindow messageWindow(sf::VideoMode(400, 200), "Mensaje");
    sf::Text messageText;
    messageText.setFont(font);
    messageText.setString(message);
    messageText.setCharacterSize(24);
    messageText.setFillColor(sf::Color::Black);
    messageText.setPosition(50, 80); // Centrar el texto en la ventana

    // Animación simple de parpadeo
    bool show = true;
    sf::Clock clock;
    while (messageWindow.isOpen()) {
        sf::Event event;
        while (messageWindow.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                messageWindow.close();
            }
        }

        // Parpadeo del texto
        if (clock.getElapsedTime().asSeconds() > 0.5) {
            show = !show;
            clock.restart();
        }

        messageWindow.clear(sf::Color::White);
        if (show) {
            messageWindow.draw(messageText);
        }
        messageWindow.display();
    }
}
