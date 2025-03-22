#include <SDL.h>
#include <SDL_image.h>
#include <vector>
#include <ctime>
#include <cstdlib>
#include <iostream>

// Cấu hình màn hình
const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 600;
const int GRID_SIZE = 20;

enum Direction { UP, DOWN, LEFT, RIGHT };

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

    bool running;
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
};

// =========================
//  Hàm khởi tạo & giải phóng
// =========================
SnakeGame::SnakeGame() {
    SDL_Init(SDL_INIT_VIDEO);
    IMG_Init(IMG_INIT_PNG);

    window = SDL_CreateWindow("Snake Game", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    snakeTexture = LoadTexture("snake.png");
    foodTexture = LoadTexture("food.png");
    wallTexture = LoadTexture("wall.png");

    if (!snakeTexture || !foodTexture || !wallTexture) {
        std::cerr << "Failed to load textures!" << std::endl;
    }

    Reset();
}

SnakeGame::~SnakeGame() {
    SDL_DestroyTexture(snakeTexture);
    SDL_DestroyTexture(foodTexture);
    SDL_DestroyTexture(wallTexture);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
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
        food.x = (rand() % (SCREEN_WIDTH / GRID_SIZE)) * GRID_SIZE;
        food.y = (rand() % (SCREEN_HEIGHT / GRID_SIZE)) * GRID_SIZE;

        // Kiểm tra xem thức ăn có nằm trên tường không
        validPosition = true;
        for (const auto& wall : walls) {
            if (food.x == wall.x && food.y == wall.y) {
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
            running = false;
        }
    }

    // Kiểm tra va chạm với thân rắn
    for (int i = 1; i < snake.size(); ++i) {
        if (newHead.x == snake[i].x && newHead.y == snake[i].y) {
            running = false;
        }
    }

    // Kiểm tra va chạm với thức ăn
    if (newHead.x == food.x && newHead.y == food.y) {
        grow = true;
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
    while (running) {
        ProcessInput();
        Update();
        Render();
        SDL_Delay(100);
    }
}

// =========================
//  Hàm `main`
// =========================
int main(int argc, char* argv[]) {
    SnakeGame game;
    game.Run();
    return 0;
}
