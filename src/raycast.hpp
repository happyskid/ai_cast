#ifndef RAYCAST_HPP
#define RAYCAST_HPP
#define NOMINMAX

#include <iostream>
#include <cmath>
#include <vector>
#include <algorithm>

const int screenWidth = 80;
const int screenHeight = 24;
const double fov = 3.14159 / 3.0;
const double depth = 100.0;

struct Vector2 {
    double x, y;

    Vector2(double x = 0, double y = 0) : x(x), y(y) {}

    Vector2 operator+(const Vector2& other) const {
        return Vector2(x + other.x, y + other.y);
    }

    Vector2 operator-(const Vector2& other) const {
        return Vector2(x - other.x, y - other.y);
    }

    Vector2 operator*(double scalar) const {
        return Vector2(x * scalar, y * scalar);
    }

    double length() const {
        return std::sqrt(x * x + y * y);
    }

    Vector2 normalized() const {
        double len = length();
        return Vector2(x / len, y / len);
    }
};

struct Player {
    Vector2 position;
    double angle;

    Player(double x = 0, double y = 0, double angle = 0) : position(x, y), angle(angle) {}
};

struct Ray {
    Vector2 origin;
    Vector2 direction;

    Ray(const Vector2& origin, const Vector2& direction) : origin(origin), direction(direction.normalized()) {}
};

void renderToConsole(const char* screenBuffer, Player player, std::vector<std::string> map) {
    const std::string shadingCharacters = "@%#*+=-:. ";

    const int numShadingCharacters = shadingCharacters.size();

    // Render the walls ('#') from the screen buffer with shading
    for (int y = 0; y < screenHeight; ++y) {
        for (int x = 0; x < screenWidth; ++x) {
            char pixel = screenBuffer[y * screenWidth + x];
            int distance = y / 2;  // Adjust the distance factor based on desired effect

            if (pixel == '#') {
                int shadingIndex = static_cast<int>((distance / static_cast<double>(screenHeight)) * numShadingCharacters);
                shadingIndex = std::min(std::max(shadingIndex, 0), numShadingCharacters - 1);

                // Render the shaded wall character
                std::cout << shadingCharacters[shadingIndex];
            }
            else {
                if (y > screenHeight / 2) {
                    std::cout << '.';
                }
                else {
                    std::cout << '`';
                }
            }
        }
        std::cout << '\n';
    }
}


void raycast(const Player& player, const std::vector<std::string>& map, char* screenBuffer) {
    int mapWidth = map[0].length();
    int mapHeight = map.size();

    for (int x = 0; x < screenWidth; ++x) {
        double rayAngle = (player.angle - fov / 2.0) + (x / static_cast<double>(screenWidth)) * fov;

        Vector2 rayDirection(std::cos(rayAngle), std::sin(rayAngle));
        Ray ray(player.position, rayDirection);

        Vector2 deltaDist(std::abs(1.0 / ray.direction.x), std::abs(1.0 / ray.direction.y));

        int mapX = static_cast<int>(ray.origin.x);
        int mapY = static_cast<int>(ray.origin.y);

        double sideDistX;
        double sideDistY;

        double perpWallDist;

        int stepX;
        int stepY;

        if (ray.direction.x < 0) {
            stepX = -1;
            sideDistX = (ray.origin.x - mapX) * deltaDist.x;
        }
        else {
            stepX = 1;
            sideDistX = (mapX + 1.0 - ray.origin.x) * deltaDist.x;
        }

        if (ray.direction.y < 0) {
            stepY = -1;
            sideDistY = (ray.origin.y - mapY) * deltaDist.y;
        }
        else {
            stepY = 1;
            sideDistY = (mapY + 1.0 - ray.origin.y) * deltaDist.y;
        }

        bool hitWall = false;
        bool hitVerticalWall = false;

        while (!hitWall) {
            if (sideDistX < sideDistY) {
                sideDistX += deltaDist.x;
                mapX += stepX;
                hitVerticalWall = true;
            }
            else {
                sideDistY += deltaDist.y;
                mapY += stepY;
                hitVerticalWall = false;
            }

            if (mapX >= 0 && mapY >= 0 && mapX < mapWidth && mapY < mapHeight) {
                char mapTile = map[mapY][mapX];
                if (mapTile == '#') {
                    hitWall = true;
                }
            }
            else {
                hitWall = true;
            }
        }

        if (hitVerticalWall) {
            perpWallDist = (mapX - ray.origin.x + (1 - stepX) / 2.0) / ray.direction.x;
        }
        else {
            perpWallDist = (mapY - ray.origin.y + (1 - stepY) / 2.0) / ray.direction.y;
        }

        int lineHeight = static_cast<int>(screenHeight / perpWallDist);

        int drawStart = -lineHeight / 2 + screenHeight / 2;
        if (drawStart < 0) {
            drawStart = 0;
        }

        int drawEnd = lineHeight / 2 + screenHeight / 2;
        if (drawEnd >= screenHeight) {
            drawEnd = screenHeight - 1;
        }

        char wallChar = '#';

        for (int y = 0; y < screenHeight; ++y) {
            if (y < drawStart) {
                screenBuffer[y * screenWidth + x] = '.';
            }
            else if (y > drawEnd) {
                screenBuffer[y * screenWidth + x] = '.';
            }
            else if (y >= drawStart && y <= drawEnd) {
                screenBuffer[y * screenWidth + x] = wallChar;
            }
        }
    }
}

#endif // RAYCAST_HPP
