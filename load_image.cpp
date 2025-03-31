#include"snakegame.h"
// hàm tải hình ảnh xuống
SDL_Texture* SnakeGame::LoadTexture(const char* path) {
    SDL_Surface* surface = IMG_Load(path);
    if (!surface) {
        cerr << "Failed to load image: " << path << " " << IMG_GetError() << endl;
        return nullptr;
    }
    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);
    return texture;
}
