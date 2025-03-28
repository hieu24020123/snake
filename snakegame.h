#ifndef SNAKEGAME_H
#define SNAKEGAME_H
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_mixer.h>
#include <vector>
#include <ctime>
#include <cstdlib>
#include <iostream>
#include<fstream>

// Cấu hình màn hình
const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 600;
const int GRID_SIZE = 20;

enum Direction { UP, DOWN, LEFT, RIGHT };
enum GameState { MENU, PLAYING, PAUSED };
struct Point {
    int x, y;
};

class SnakeGame {
public:
    SnakeGame();
    ~SnakeGame();
    void Run();

private:
    SDL_Window* window;
    SDL_Renderer* renderer;
    SDL_Texture* snakeTexture;
    SDL_Texture* foodTexture;
    SDL_Texture* wallTexture;
    SDL_Texture* menuBackground;
    SDL_Texture* gameBackground;
    SDL_Texture* newGameButton;
    SDL_Texture* continueButton;
    SDL_Texture* youlose;
    SDL_Texture* load;
    Mix_Music* backgroundMusic;
    Mix_Chunk* eatSound;
    Mix_Chunk* loseSound;

    bool running;
    GameState gameState;
    Direction direction;
    std::vector<Point> snake;
    std::vector<Point> walls;
    Point food;
    bool grow;

    SDL_Texture* LoadTexture(const char* path);
    void ProcessInput();
    void Update();
    void Render();
    void Reset();
    void SpawnFood();
    void CreateWalls();
    void ShowMenu();
    void SaveGame();
    void LoadGame();
};
#endif // SNAKEGAME_H
