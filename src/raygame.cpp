#define NOMINMAX
#include <iostream>
#include <cstdlib> // For the system function
#include <conio.h> // For non-blocking keyboard input
#include <windows.h>
#include "raycast.hpp"

void clearScreen()
{
    COORD cursorPosition;	cursorPosition.X = 0;	cursorPosition.Y = 0;	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), cursorPosition);
}

int main() {
    const int screenWidth = 80;
    const int screenHeight = 24;

    Player player(8.0, 8.0, 0.0);

    std::vector<std::string> map = {
        "#######################",
        "#.............#.......#",
        "#.............#.......#",
        "#.............#.......#",
        "#.............#.......#",
        "#.....................#",
        "#.....................#",
        "#.............#########",
        "#.............#.......#",
        "#.............#.......#",
        "#.............#.......#",
        "#.............#.......#",
        "#.............#.......#",
        "##........#####.......#",
        "#.............#.......#",
        "#.............##......#",
        "#.............#.......#",
        "#.............#.......#",
        "#.....................#",
        "#.....................#",
        "#.............#.......#",
        "#.............#.......#",
        "#.............#.......#",
        "#######################"
    };

    char* screenBuffer = new char[screenWidth * screenHeight];

    while (true) {
        clearScreen();
        // Clear the screen buffer
        for (int i = 0; i < screenWidth * screenHeight; ++i) {
            screenBuffer[i] = ' ';
        }

        // Perform raycasting
        raycast(player, map, screenBuffer);

        // Render the screen buffer
        renderToConsole(screenBuffer, player, map);

        // Simulate player movement and rotation
        double rotationSpeed = 0.1;
        double moveSpeed = 0.1;

        if (_kbhit()) { // Check if a key is pressed
            char input = _getch(); // Get the pressed key without waiting for Enter

            if (input == 'a') {
                player.angle -= rotationSpeed;
            }
            else if (input == 'd') {
                player.angle += rotationSpeed;
            }
            else if (input == 'w') {
                player.position.x += std::cos(player.angle) * moveSpeed;
                player.position.y += std::sin(player.angle) * moveSpeed;
            }
            else if (input == 's') {
                player.position.x -= std::cos(player.angle) * moveSpeed;
                player.position.y -= std::sin(player.angle) * moveSpeed;
            }
        }
    }

    delete[] screenBuffer;
    return 0;
}
