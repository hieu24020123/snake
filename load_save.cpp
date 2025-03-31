#include"snakegame.h"
// hàm lưu game
void SnakeGame::SaveGame() {
    ofstream file("savegame.txt");
    if (file.is_open()) {
        file << direction << " " << food.x << " " << food.y << " " << snake.size() << "\n";
        for (const auto& segment : snake) {
            file << segment.x << " " << segment.y << "\n";
        }
        file.close();
    }
}
// hàm tải game
void SnakeGame::LoadGame() {
    ifstream file("savegame.txt");
    int snakeSize;
    int dir;

    if (!(file >> dir >> food.x >> food.y >> snakeSize)) {
        cerr << "Lỗi đọc dữ liệu từ file!\n";
        return;
    }

    direction = static_cast<Direction>(dir);
    snake.clear();

    for (int i = 0; i < snakeSize; i++) {
        Point segment;
        if (!(file >> segment.x >> segment.y)) {
            cerr << "Lỗi khi đọc vị trí của rắn!\n";
            return;
        }
        snake.push_back(segment);
    }

    file.close();
    grow = false;  // Đảm bảo trạng thái đúng sau khi tải
}
