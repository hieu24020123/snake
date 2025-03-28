#include"snakegame.h"
void SnakeGame::CreateWalls() {
    walls.clear();

    // Tạo tường viền màn hình
    for (int x = 0; x < SCREEN_WIDTH; x += GRID_SIZE) {
        walls.push_back({ x, 0 });
        walls.push_back({ x, SCREEN_HEIGHT - GRID_SIZE });
    }

    for (int y = 0; y < SCREEN_HEIGHT; y += GRID_SIZE) {
        walls.push_back({ 0, y });
        walls.push_back({ SCREEN_WIDTH - GRID_SIZE, y });
    }
}
void SnakeGame::Render() {
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);

    // vẽ backgrond
    SDL_Rect bg_gamerect{0,0,SCREEN_WIDTH,SCREEN_HEIGHT};
    SDL_RenderCopy (renderer,gameBackground,nullptr, &bg_gamerect);

    // Vẽ tường
    for (const auto& wall : walls) {
        SDL_Rect rect = { wall.x, wall.y, GRID_SIZE, GRID_SIZE };
        SDL_RenderCopy(renderer, wallTexture, nullptr, &rect);
    }

    // Vẽ rắn
    for (auto& segment : snake) {
        SDL_Rect rect = { segment.x, segment.y, GRID_SIZE, GRID_SIZE };
        SDL_RenderCopy(renderer, snakeTexture, nullptr, &rect);
    }

    // Vẽ thức ăn
    SDL_Rect foodRect = { food.x, food.y, GRID_SIZE, GRID_SIZE };
    SDL_RenderCopy(renderer, foodTexture, nullptr, &foodRect);

    SDL_RenderPresent(renderer);
}
