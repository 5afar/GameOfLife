#ifndef GAME_HPP
#define GAME_HPP
#include "game_constants.hpp"
#include <SDL2/SDL.h>
#include <vector>
#include <iostream>
#include <unistd.h>

class Game
{
public:
    Game() = default;
    
    Game(int argc, char *argv[]);
    ~Game() = default;

    void start();

private:
    int screenWidth = DEFAULT_SCREEN_WIDTH;
    int screenHeight = DEFAULT_SCREEN_HEIGHT;
    int cellSize = DEFAULT_CELL_SIZE;
    int gridWidth = screenWidth / cellSize;
    int gridHeight = screenHeight / cellSize;
    int speed = DEFAULT_SPEED;

    void parseArguments(int argc, char *argv[]);
    void renderGrid(SDL_Renderer* renderer, const std::vector<std::vector<bool>>& grid) ;
    int countLiveNeighbors(const std::vector<std::vector<bool>>& grid, int x, int y) ;
    void updateGrid(std::vector<std::vector<bool>>& grid) ;
    void toggleCell(std::vector<std::vector<bool>>& grid, int x, int y);
};

#endif