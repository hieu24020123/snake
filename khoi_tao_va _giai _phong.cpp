#include"snakegame.h"
SnakeGame::SnakeGame() {
    SDL_Init(SDL_INIT_VIDEO);
    IMG_Init(IMG_INIT_PNG);
    Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048); // Thêm dòng này

    window = SDL_CreateWindow("Snake Game", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    menuBackground = LoadTexture("menu_bg.png");
    gameBackground = LoadTexture("bg_game.png");
    newGameButton = LoadTexture("new_game.png");
    continueButton = LoadTexture("continue.png");
    snakeTexture = LoadTexture("snake.png");
    foodTexture = LoadTexture("food.png");
    wallTexture = LoadTexture("wall.png");
    youlose= LoadTexture("you_lose.png");
    load= LoadTexture("load.png");
    backgroundMusic = Mix_LoadMUS("beach.mp3");
    eatSound = Mix_LoadWAV("eat.mp3");
    loseSound=Mix_LoadWAV("lose.mp3");

    if (!menuBackground || !newGameButton || !continueButton ||!snakeTexture || !foodTexture || !wallTexture || !youlose) {
        std::cerr << "Failed to load textures!" << std::endl;
    }
    if (!backgroundMusic || !eatSound || !loseSound) {
        std::cerr << "Failed to load music: " << Mix_GetError() << std::endl;
    } else {
        Mix_PlayMusic(backgroundMusic, -1); // Phát nhạc lặp vô hạn
    }
    gameState = MENU;
    running = true;

    Reset();
}

SnakeGame::~SnakeGame() {
    SDL_DestroyTexture(menuBackground);
    SDL_DestroyTexture(gameBackground);
    SDL_DestroyTexture(newGameButton);
    SDL_DestroyTexture(continueButton);
    SDL_DestroyTexture(snakeTexture);
    SDL_DestroyTexture(foodTexture);
    SDL_DestroyTexture(wallTexture);
    SDL_DestroyTexture(youlose);
    SDL_DestroyTexture(load);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    Mix_FreeChunk(eatSound);
    Mix_FreeChunk(loseSound);
    Mix_CloseAudio(); // Tắt âm thanh
    IMG_Quit();
    SDL_Quit();
}
