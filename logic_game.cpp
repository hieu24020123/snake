#include"snakegame.h"
void SnakeGame::Reset() {
    snake.clear();
    snake.push_back({ SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2 });
    direction = RIGHT;
    grow = false;
    SpawnFood();
    CreateWalls();
    running = true;
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
            }
        }
    }
}
// hàm xử lý đầu vào
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
