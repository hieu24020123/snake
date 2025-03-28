#include"snakegame.h"
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
