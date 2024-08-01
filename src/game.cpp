#include "game.hpp"

Game::Game(int argc, char *argv[])
{
    int opt;
    while ((opt = getopt(argc, argv, "w:h:c:s:")) != -1)
    {
        switch (opt)
        {
        case 'w':
            screenWidth = atoi(optarg);
            break;
        case 'h':
            screenHeight = atoi(optarg);
            break;
        case 'c':
            cellSize = atoi(optarg);
            break;
        case 's':
            speed = atoi(optarg);
            break;
        default:
            std::cerr << "Usage: " << argv[0] << " [-w screen_width] [-h screen_height] [-c cell_size] [-s speed_ms]" << std::endl;
            exit(EXIT_FAILURE);
        }
    }

    gridWidth = screenWidth / cellSize;
    gridHeight = screenHeight / cellSize;
}

void Game::parseArguments(int argc, char *argv[])
{
    int opt;
    while ((opt = getopt(argc, argv, "w:h:c:s:")) != -1)
    {
        switch (opt)
        {
        case 'w':
            screenWidth = atoi(optarg);
            break;
        case 'h':
            screenHeight = atoi(optarg);
            break;
        case 'c':
            cellSize = atoi(optarg);
            break;
        case 's':
            speed = atoi(optarg);
            break;
        default:
            std::cerr << "Usage: " << argv[0] << " [-w screen_width] [-h screen_height] [-c cell_size] [-s speed_ms]" << std::endl;
            exit(EXIT_FAILURE);
        }
    }

    gridWidth = screenWidth / cellSize;
    gridHeight = screenHeight / cellSize;
}
// С этой функции начинает работу игра
void Game::start()
{
    // Инициализация библиотеки
    if (SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        std::cerr << "SDL could not initialize! SDL_Error: " << SDL_GetError() << std::endl;
        return;
    }

    // Создание окна
    SDL_Window *window = SDL_CreateWindow("Game of Life", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, screenWidth, screenHeight, SDL_WINDOW_SHOWN);
    if (window == nullptr)
    {
        std::cerr << "Window could not be created! SDL_Error: " << SDL_GetError() << std::endl;
        SDL_Quit();
        return;
    }
    // Создание контекста отрисовки и привязка его к окну
    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (renderer == nullptr)
    {
        std::cerr << "Renderer could not be created! SDL_Error: " << SDL_GetError() << std::endl;
        SDL_DestroyWindow(window);
        SDL_Quit();
        return;
    }
    // Матрица живых клеток
    std::vector<std::vector<bool>> grid(gridHeight, std::vector<bool>(gridWidth, false));

    bool quit = false;
    bool paused = true;
    SDL_Event e;

    while (!quit)
    {
        // Отлавливание событий в окне
        while (SDL_PollEvent(&e) != 0)
        {
            // Обработка завршения программы
            if (e.type == SDL_QUIT)
            {
                quit = true;
            }
            // Обработка нажатий мыши
            else if (e.type == SDL_MOUSEBUTTONDOWN)
            {
                int x, y;
                // Получаем координаты, где произошло событие нажатия кнопки мыши
                SDL_GetMouseState(&x, &y);
                // Активация/деактивация клетки в заданной ячейке
                toggleCell(grid, x, y);
            }
            // Обработка нажатия space для установки паузы
            else if (e.type == SDL_KEYDOWN)
            {
                if (e.key.keysym.sym == SDLK_SPACE)
                {
                    paused = !paused;
                }
            }
        }
        // Если активирована пауза, то просчет следующей сетки не происходит
        if (!paused)
        {
            updateGrid(grid);
        }
        // Отрисовка текущего состояния сетки
        renderGrid(renderer, grid);
        SDL_Delay(speed);
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}

// Функция изменяет состояние клетки в точке, на которую щелкнули мышкой
void Game::toggleCell(std::vector<std::vector<bool>> &grid, int x, int y)
{
    int gridX = x / cellSize;
    int gridY = y / cellSize;
    // Если точка находится внутри окна, то меняем состояние клетки
    if (gridX >= 0 && gridX < gridWidth && gridY >= 0 && gridY < gridHeight)
    {
        grid[gridY][gridX] = !grid[gridY][gridX];
    }
}

// Функция пересчитывает состояние сетки
void Game::updateGrid(std::vector<std::vector<bool>> &grid)
{
    std::vector<std::vector<bool>> newGrid = grid;
    for (int i = 0; i < gridHeight; ++i)
    {
        for (int j = 0; j < gridWidth; ++j)
        {
            // Узнаем количество живых соседей
            int liveNeighbors = countLiveNeighbors(grid, i, j);
            // Если клетка живая
            if (grid[i][j])
            {
                // Если живых соседей 2 или 3, то клетка остается живая, иначе умирает
                newGrid[i][j] = liveNeighbors == 2 || liveNeighbors == 3;
            }
            // Если клетка мертвая
            else
            {
                // Если количество живых соседей 3, то клетка становится живой
                newGrid[i][j] = liveNeighbors == 3;
            }
        }
    }
    grid = newGrid;
}

// Функция подсчитывает количество живых соседей у заданной клетки
int Game::countLiveNeighbors(const std::vector<std::vector<bool>> &grid, int x, int y)
{
    int liveNeighbors = 0;
    for (int i = -1; i <= 1; ++i)
    {
        for (int j = -1; j <= 1; ++j)
        {
            // i==0 && j==0 Это текущая клетка, для которой считаются соседи
            if (i == 0 && j == 0)
                continue;
            // Узнаем координату соседа
            int nx = x + i, ny = y + j;
            // Проверяем его на принадлежность к сетке
            if (nx >= 0 && nx < gridHeight && ny >= 0 && ny < gridWidth)
            {
                // Если true вектор вернет единицу и добавит ее, если false вектор вернет 0
                liveNeighbors += grid[nx][ny];
            }
        }
    }
    return liveNeighbors;
}

// Функция отрисовывает сетку в окне
void Game::renderGrid(SDL_Renderer *renderer, const std::vector<std::vector<bool>> &grid)
{
    // Очищаем экран
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderClear(renderer);

    // Расставляем зеленым цветом живые клетки
    SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
    for (int i = 0; i < gridHeight; ++i)
    {
        for (int j = 0; j < gridWidth; ++j)
        {
            if (grid[i][j])
            {
                SDL_Rect cell = {j * cellSize, i * cellSize, cellSize, cellSize};
                SDL_RenderFillRect(renderer, &cell);
            }
        }
    }

    // Отрисовываем сетку
    SDL_SetRenderDrawColor(renderer, 200, 200, 200, 255);
    for (int i = 0; i <= gridHeight; ++i)
    {
        SDL_RenderDrawLine(renderer, 0, i * cellSize, screenWidth, i * cellSize);
    }
    for (int j = 0; j <= gridWidth; ++j)
    {
        SDL_RenderDrawLine(renderer, j * cellSize, 0, j * cellSize, screenHeight);
    }

    SDL_RenderPresent(renderer);
}