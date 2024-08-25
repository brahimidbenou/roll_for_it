#include "roll_for_it.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>


void draw_dice(SDL_Renderer *renderer, SDL_Texture *textures[], joueur *player) {
    for (int i = 0; i < NB_DES; i++) {
        SDL_Rect destRect = { i * DICE_SIZE, 50, DICE_SIZE, DICE_SIZE };
        SDL_RenderCopy(renderer, textures[player->des[i][0] - 1], NULL, &destRect);
    }
}


int main(int argc, char *argv[]) {
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
        return 1;
    }

    SDL_Window *window = SDL_CreateWindow("Lancer de dés",
                                          SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
                                          WINDOW_WIDTH, WINDOW_HEIGHT,
                                          SDL_WINDOW_SHOWN);
    if (!window) {
        printf("Window could not be created! SDL_Error: %s\n", SDL_GetError());
        SDL_Quit();
        return 1;
    }

    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (!renderer) {
        printf("Renderer could not be created! SDL_Error: %s\n", SDL_GetError());
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }

    // Load dice textures
    SDL_Texture *diceTextures[6];
    for (int i = 0; i < 6; i++) {
        char filename[30];
        snprintf(filename, sizeof(filename), "SDL_image/dice/R%d.jpg", i + 1);
        printf("Trying to load texture: %s\n", filename);  // Debug message
        diceTextures[i] = IMG_LoadTexture(renderer, filename);
        if (!diceTextures[i]) {
            printf("Failed to load texture %s! SDL_Error: %s\n", filename, SDL_GetError());
            for (int j = 0; j < i; j++) {
                SDL_DestroyTexture(diceTextures[j]);
            }
            SDL_DestroyRenderer(renderer);
            SDL_DestroyWindow(window);
            SDL_Quit();
            return 1;
        }
    }

    // Load button texture
    SDL_Texture *buttonTexture = IMG_LoadTexture(renderer, "SDL_image/roll.png");
    if (!buttonTexture) {
        printf("Failed to load button texture! SDL_Error: %s\n", SDL_GetError());
        for (int i = 0; i < 6; i++) {
            SDL_DestroyTexture(diceTextures[i]);
        }
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }
    SDL_Rect buttonRect = { (WINDOW_WIDTH - DICE_SIZE) / 2, 135, DICE_SIZE-10, DICE_SIZE-10};

    srand(time(NULL));
    joueur player = {0};

    lancer_des(&player);

    int quit = 0;
    SDL_Event e;
    while (!quit) {
        while (SDL_PollEvent(&e) != 0) {
            if (e.type == SDL_QUIT) {
                quit = 1;
            } else if (e.type == SDL_KEYDOWN) {
                if (e.key.keysym.sym == SDLK_SPACE) {
                    lancer_des(&player);
                }
            } else if (e.type == SDL_MOUSEBUTTONDOWN) {
                int x, y;
                SDL_GetMouseState(&x, &y);
                if (x >= buttonRect.x && x <= buttonRect.x + buttonRect.w && y >= buttonRect.y && y <= buttonRect.y + buttonRect.h) {
                    lancer_des(&player);
                }
            }
        }

        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        SDL_RenderClear(renderer);

        draw_dice(renderer, diceTextures, &player);
        SDL_RenderCopy(renderer, buttonTexture, NULL, &buttonRect);

        SDL_RenderPresent(renderer);
    }

    SDL_DestroyTexture(buttonTexture);
    for (int i = 0; i < 6; i++) {
        SDL_DestroyTexture(diceTextures[i]);
    }
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}
