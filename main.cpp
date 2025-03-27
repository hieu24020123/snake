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

// =========================
//  Hàm khởi tạo & giải phóng
// =========================
SnakeGame::SnakeGame() {
    SDL_Init(SDL_INIT_VIDEO);
    IMG_Init(IMG_INIT_PNG);
    Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048); // Thêm dòng này

    window = SDL_CreateWindow("Snake Game", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    menuBackground = LoadTexture("menu_bg.png");
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

// =========================
//  Hàm hỗ trợ
// =========================
SDL_Texture* SnakeGame::LoadTexture(const char* path) {
    SDL_Surface* surface = IMG_Load(path);
    if (!surface) {
        std::cerr << "Failed to load image: " << path << " " << IMG_GetError() << std::endl;
        return nullptr;
    }
    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);
    return texture;
}

// =========================
//  Cài đặt ban đầu
// =========================
void SnakeGame::Reset() {
    snake.clear();
    snake.push_back({ SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2 });
    direction = RIGHT;
    grow = false;
    SpawnFood();
    CreateWalls();
    running = true;
}
void SnakeGame::ShowMenu() {
    bool inMenu = true;
    SDL_Event event;

    while (inMenu) {
        SDL_RenderClear(renderer);
        SDL_Rect bgRect = { 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT };
        SDL_RenderCopy(renderer, menuBackground, nullptr, &bgRect);

        SDL_Rect newGameRect = { SCREEN_WIDTH / 3, 200, 250, 80 };
        SDL_RenderCopy(renderer, newGameButton, nullptr, &newGameRect);

        SDL_Rect continueRect = { SCREEN_WIDTH / 3, 320, 250, 80 };
        SDL_RenderCopy(renderer, continueButton, nullptr, &continueRect);

         SDL_Rect loadRect = { SCREEN_WIDTH / 3, 440, 250, 80 };
        SDL_RenderCopy(renderer, load, nullptr, &loadRect);

        SDL_RenderPresent(renderer);

        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                running = false;
                inMenu = false;
            }
            if (event.type == SDL_MOUSEBUTTONDOWN) {
                int x = event.button.x;
                int y = event.button.y;

                if (x > newGameRect.x && x < newGameRect.x + newGameRect.w && y > newGameRect.y && y < newGameRect.y + newGameRect.h) {
                    Reset();
                    gameState = PLAYING;
                    inMenu = false;
                }
                if (x > continueRect.x && x < continueRect.x + continueRect.w && y > continueRect.y && y < continueRect.y + continueRect.h) {
                    gameState = PLAYING;
                    inMenu = false;
                }
                 if (x > loadRect.x && x < loadRect.x + loadRect.w && y > loadRect.y && y < loadRect.y + loadRect.h) {
                    LoadGame();
                    gameState = PLAYING;
                    inMenu = false;
                }

            }
        }
    }
}

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

void SnakeGame::SpawnFood() {
    srand(time(0));
    bool validPosition = false;

    while (!validPosition) {
        // Đặt thức ăn trong khoảng an toàn, tránh tường
        food.x = (rand() % ((SCREEN_WIDTH / GRID_SIZE) - 2) + 1) * GRID_SIZE;
        food.y = (rand() % ((SCREEN_HEIGHT / GRID_SIZE) - 2) + 1) * GRID_SIZE;

        validPosition = true;  // Mặc định vị trí hợp lệ, kiểm tra tiếp

        // Kiểm tra xem thức ăn có trùng với thân rắn không
        for (auto& segment : snake) {
            if (segment.x == food.x && segment.y == food.y) {
                validPosition = false;
                break;
            }
        }
    }
}

// =========================
//  Xử lý nhập từ bàn phím
// =========================
void SnakeGame::ProcessInput() {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        if (event.type == SDL_QUIT) {
            running = false;
        }
        if (event.type == SDL_KEYDOWN) {
            switch (event.key.keysym.sym) {
                case SDLK_UP: if (direction != DOWN) direction = UP; break;
                case SDLK_DOWN: if (direction != UP) direction = DOWN; break;
                case SDLK_LEFT: if (direction != RIGHT) direction = LEFT; break;
                case SDLK_RIGHT: if (direction != LEFT) direction = RIGHT; break;
                case SDLK_SPACE: // Nhấn Space để quay lại menu
                    if (gameState == PLAYING) {
                        gameState = MENU;
                    } break;
                case SDLK_s:
                    SaveGame();
                    break;
            }
        }
    }
}

// =========================
//  Cập nhật trạng thái game
// =========================
void SnakeGame::Update() {
    Point newHead = snake[0];

    switch (direction) {
        case UP: newHead.y -= GRID_SIZE; break;
        case DOWN: newHead.y += GRID_SIZE; break;
        case LEFT: newHead.x -= GRID_SIZE; break;
        case RIGHT: newHead.x += GRID_SIZE; break;
    }

    // Kiểm tra va chạm với tường
    for (const auto& wall : walls) {
        if (newHead.x == wall.x && newHead.y == wall.y) {
            Mix_PlayChannel(-1, loseSound, 0);
    for (int i = 0; i < 5; i++) {
            SDL_RenderClear(renderer);
            SDL_Rect ylRect = { 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT };
            SDL_RenderCopy(renderer, youlose, nullptr, &ylRect);
            SDL_RenderPresent(renderer); // Cập nhật màn hình
            SDL_Delay(200);
        }
             gameState = MENU; // Quay về menu
        return;
        }
    }

    // Kiểm tra va chạm với thân rắn
    for (int i = 1; i < snake.size(); ++i) {
        if (newHead.x == snake[i].x && newHead.y == snake[i].y) {
            Mix_PlayChannel(-1, loseSound, 0);
            for (int i = 0; i < 5; i++) {
            SDL_RenderClear(renderer); // Xóa màn hình trước khi vẽ
            SDL_Rect ylRect = { 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT };
            SDL_RenderCopy(renderer, youlose, nullptr, &ylRect);
            SDL_RenderPresent(renderer); // Cập nhật màn hình
            SDL_Delay(200);
        }
             gameState = MENU; // Quay về menu
        return;
        }
    }

    // Kiểm tra va chạm với thức ăn
    if (newHead.x == food.x && newHead.y == food.y) {
        grow = true;
        Mix_PlayChannel(-1, eatSound, 0); // âm thanh lúc ăn
        SpawnFood();
    }

    snake.insert(snake.begin(), newHead);
    if (!grow) {
        snake.pop_back();
    }
    grow = false;
}

// =========================
//  Vẽ hình ảnh lên màn hình
// =========================
void SnakeGame::Render() {
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);

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

// =========================
//  Vòng lặp chính
// =========================
void SnakeGame::Run() {
    ShowMenu();
    while (running) {
        if (gameState == MENU) {
            ShowMenu();
        } else if (gameState == PLAYING) {
            ProcessInput();
            Update();
            Render();
            SDL_Delay(100);
        }
    }
}
void SnakeGame::SaveGame() {
    std::ofstream file("savegame.txt");
    if (file.is_open()) {
        file << direction << " " << food.x << " " << food.y << " " << snake.size() << "\n";
        for (const auto& segment : snake) {
            file << segment.x << " " << segment.y << "\n";
        }
        file.close();
    }
}
void SnakeGame::LoadGame() {
    std::ifstream file("savegame.txt");
    if (!file.is_open()) {
        std::cerr << "Không thể mở file savegame.txt!\n";
        return;
    }

    int snakeSize;
    int dir;

    if (!(file >> dir >> food.x >> food.y >> snakeSize)) {
        std::cerr << "Lỗi đọc dữ liệu từ file!\n";
        return;
    }

    // Kiểm tra giá trị hợp lệ
    if (dir < UP || dir > RIGHT || snakeSize <= 0) {
        std::cerr << "Dữ liệu trong file không hợp lệ!\n";
        return;
    }

    direction = static_cast<Direction>(dir);
    snake.clear();

    for (int i = 0; i < snakeSize; i++) {
        Point segment;
        if (!(file >> segment.x >> segment.y)) {
            std::cerr << "Lỗi khi đọc vị trí của rắn!\n";
            return;
        }
        snake.push_back(segment);
    }

    file.close();
    grow = false;  // Đảm bảo trạng thái đúng sau khi tải
}

// =========================
//  Hàm main
// =========================
int main(int argc, char* argv[]) {
    SnakeGame game;
    game.Run();
    return 0;
}
