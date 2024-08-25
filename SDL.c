//#include "roll_for_it.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdio.h>
#include <string.h>

#define WINDOW_WIDTH 900
#define WINDOW_HEIGHT 600

void end_sdl(char ok, const char* msg, SDL_Window* window, SDL_Renderer* renderer) {
    char msg_formated[255];
    int l;

    if (!ok) {
        strncpy(msg_formated, msg, 250);
        l = strlen(msg_formated);
        strcpy(msg_formated + l, " : %s\n");

        SDL_Log(msg_formated, SDL_GetError());
    }

    if (renderer != NULL) {
        SDL_DestroyRenderer(renderer);
        renderer = NULL;
    }
    if (window != NULL) {
        SDL_DestroyWindow(window);
        window = NULL;
    }

    SDL_Quit();

    if (!ok) {
        exit(EXIT_FAILURE);
    }
}

SDL_Texture* load_texture_from_image(const char* file_image_name, SDL_Window* window, SDL_Renderer* renderer) {
    SDL_Surface* my_image = IMG_Load(file_image_name);
    if (my_image == NULL) {
        end_sdl(0, "Chargement de l'image impossible", window, renderer);
    }

    SDL_Texture* my_texture = SDL_CreateTextureFromSurface(renderer, my_image);
    SDL_FreeSurface(my_image);
    if (my_texture == NULL) {
        end_sdl(0, "Echec de la transformation de la surface en texture", window, renderer);
    }

    return my_texture;
}

/*
int afficherFenetreDemarrage(SDL_Renderer *renderer, SDL_Window* window) {
    SDL_bool continuer = SDL_TRUE;
    SDL_Event evenement;

    // Charger une texture pour l'arrière-plan
    SDL_Texture* Start_Background = load_texture_from_image("SDL_image/background.jpg", window, renderer);
    if (!Start_Background) {
        fprintf(stderr, "Erreur lors du chargement de la texture de l'arrière-plan: %s\n", SDL_GetError());
        return -1;
    }

    while (continuer) {
        while (SDL_PollEvent(&evenement)) {
            switch (evenement.type) {
                case SDL_QUIT:
                    continuer = SDL_FALSE;
                    break;
                case SDL_MOUSEBUTTONDOWN:
                    break;
            }
        }

        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);
        SDL_RenderCopy(renderer, Start_Background, NULL, NULL); // Ajouter l'arrière-plan
        SDL_RenderPresent(renderer);
    }

    SDL_DestroyTexture(Start_Background);
    return 0;
}

// Fonction pour afficher la page de sélection de mode de jeu
int afficherSelectionMode(SDL_Renderer * renderer) {
    SDL_bool continuer = SDL_TRUE;
    SDL_Event event;
    int Mode; // Mode par défaut


    // Centrer les boutons sur l'écran
    int boutonWidth = 350;
    int boutonHeight = 250;
    int boutonSpacing = 10; // Espace entre les boutons
    int totalHeight = (2 * boutonHeight) + boutonSpacing;
    int startY = (WINDOW_HEIGHT - totalHeight) / 2;

    SDL_Rect boutonPvP = {(WINDOW_WIDTH - boutonWidth) / 2, startY, boutonWidth, boutonHeight};
    SDL_Rect boutonPvC = {(WINDOW_WIDTH - boutonWidth) / 2, startY + boutonHeight + boutonSpacing, boutonWidth, boutonHeight};


    // Charger les textures pour les boutons
    SDL_Texture* text_PvP = IMG_LoadTexture(renderer, "SDL_image/PvP.jpg");
    SDL_Texture* text_PvC = IMG_LoadTexture(renderer, "SDL_image/PvC.jpg");

    // Vérifier si les textures ont été chargées correctement
    if (!text_PvP || !text_PvC) {
        fprintf(stderr, "Erreur lors du chargement des textures des boutons: %s\n", SDL_GetError());
        return -1;
    }

    while (continuer) {
        while (SDL_PollEvent(&event)) {
            switch (event.type) {
                case SDL_QUIT:
                    continuer = SDL_FALSE;
                    break;
                case SDL_MOUSEBUTTONDOWN:
                    if (event.button.x >= boutonPvP.x && event.button.x <= (boutonPvP.x + boutonPvP.w) &&
                        event.button.y >= boutonPvP.y && event.button.y <= (boutonPvP.y + boutonPvP.h)) {
                        Mode= 1; //PvP
                        continuer = SDL_FALSE;
                    } else if (event.button.x >= boutonPvC.x && event.button.x <= (boutonPvC.x + boutonPvC.w) &&
                               event.button.y >= boutonPvC.y && event.button.y <= (boutonPvC.y + boutonPvC.h)) {
                        Mode= 0; //PvC
                        continuer = SDL_FALSE;
                    }
                    break;
            }
        }


        SDL_SetRenderDrawColor(renderer, 255, 255, 255, SDL_ALPHA_OPAQUE); // White background
        SDL_RenderClear(renderer);

        SDL_RenderCopy(renderer, text_PvP, NULL, &boutonPvP);
        SDL_RenderCopy(renderer, text_PvC, NULL, &boutonPvC);
        SDL_RenderPresent(renderer);
    }

    // Libérer les textures
    SDL_DestroyTexture(text_PvP);
    SDL_DestroyTexture(text_PvC);

    return Mode;
}

int EndGame(SDL_Renderer *renderer, SDL_Window* window, int win) {
    SDL_bool continuer = SDL_TRUE;
    SDL_Event event;
    
    // Charger une texture pour l'arrière-plan
    SDL_Texture* Game_over = load_texture_from_image("SDL_image/game_over.jfif", window, renderer);
    if (!Game_over) {
        fprintf(stderr, "Erreur lors du chargement de la texture de l'arrière-plan: %s\n", SDL_GetError());
        return -1;
    }
    SDL_Texture* You_win = load_texture_from_image("SDL_image/you_win.jpg", window, renderer);
    if (!You_win) {
        fprintf(stderr, "Erreur lors du chargement de la texture de l'arrière-plan: %s\n", SDL_GetError());
        return -1;
    }

    while (continuer) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                continuer = SDL_FALSE;
            }
        }

        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);
        if(win == 1){
            SDL_RenderCopy(renderer, You_win, NULL, NULL); // Ajouter l'arrière-plan
        }
        else if (win == 2){
            SDL_RenderCopy(renderer, Game_over, NULL, NULL); 
        }
        SDL_RenderPresent(renderer);
    }

    SDL_DestroyTexture(Game_over);
    SDL_DestroyTexture(You_win);
    return 0;
}
*/

int main(){
    SDL_Init(SDL_INIT_VIDEO);

    SDL_Window *window = SDL_CreateWindow("Pentago", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_SHOWN);
    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    //afficherFenetreDemarrage(renderer,window);

    SDL_Texture* red_dice[] = {load_texture_from_image("SDL_image/dice/R1.jpg",window, renderer),load_texture_from_image("SDL_image/dice/R2.jpg",window, renderer),
                            load_texture_from_image("SDL_image/dice/R3.jpg",window, renderer),load_texture_from_image("SDL_image/dice/R4.jpg",window, renderer),
                            load_texture_from_image("SDL_image/dice/R5.jpg",window, renderer),load_texture_from_image("SDL_image/dice/R6.jpg",window, renderer)};
    
    SDL_Texture* white_dice[] = {load_texture_from_image("SDL_image/dice/W1.jpg",window, renderer),load_texture_from_image("SDL_image/dice/W2.jpg",window, renderer),
                            load_texture_from_image("SDL_image/dice/W3.jpg",window, renderer),load_texture_from_image("SDL_image/dice/W4.jpg",window, renderer),
                            load_texture_from_image("SDL_image/dice/W5.jpg",window, renderer),load_texture_from_image("SDL_image/dice/W6.jpg",window, renderer)};
    

    SDL_Texture* CARDS[] = {load_texture_from_image("SDL_image/cards/card15/0.png",window, renderer),load_texture_from_image("SDL_image/cards/card15/1.png",window, renderer),
                            load_texture_from_image("SDL_image/cards/card15/2.png",window, renderer),load_texture_from_image("SDL_image/cards/card10/3.png",window, renderer),
                            load_texture_from_image("SDL_image/cards/card10/4.png",window, renderer),load_texture_from_image("SDL_image/cards/card10/4.png",window, renderer),
                            load_texture_from_image("SDL_image/cards/card10/5.png",window, renderer),load_texture_from_image("SDL_image/cards/card10/6.png",window, renderer),
                            load_texture_from_image("SDL_image/cards/card5/7.png",window, renderer),load_texture_from_image("SDL_image/cards/card5/8.png",window, renderer),
                            load_texture_from_image("SDL_image/cards/card5/9.png",window, renderer),load_texture_from_image("SDL_image/cards/card5/10.png",window, renderer),
                            load_texture_from_image("SDL_image/cards/card5/11.png",window, renderer),load_texture_from_image("SDL_image/cards/card5/12.png",window, renderer),
                            load_texture_from_image("SDL_image/cards/card5/13.png",window, renderer),load_texture_from_image("SDL_image/cards/card5/14.png",window, renderer)};


    SDL_Texture* pass_turn = load_texture_from_image("SDL_image/pass_turn.png", window, renderer);
    SDL_Texture* return_dice = load_texture_from_image("SDL_image/return.jpg", window, renderer);

    //SDL_Texture* roll_dice = load_texture_from_image("SDL_image/roll_dice.png", window, renderer);

    int running = 1;
    SDL_Event event;
    while(running){
         while (SDL_PollEvent(&event)) {
            
            if (event.type == SDL_QUIT) {
                running = 0;
            } /*else if (event.type == SDL_MOUSEBUTTONDOWN){
                int x, y;
                SDL_GetMouseState(&x, &y);

            }*/
        }
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, SDL_ALPHA_OPAQUE); // White background
        SDL_RenderClear(renderer);

        SDL_Rect card0 = {100,75,200,350};
        SDL_Rect card1 = {350,75,200,350};
        SDL_Rect card2 = {600,75,200,350};

        SDL_Rect dice0 = {120,437,75,75};
        SDL_Rect dice1 = {237,437,75,75};
        SDL_Rect dice2 = {354,437,75,75};
        SDL_Rect dice3 = {471,437,75,75};
        SDL_Rect dice4 = {588,437,75,75};
        SDL_Rect dice5 = {705,437,75,75};

        SDL_Rect return_rect = {0,520,100,80};
        SDL_Rect pass_rect = {800,520,100,80};
        //render les cartes de la table
        SDL_RenderCopy(renderer,CARDS[0],NULL,&card0);
        SDL_RenderCopy(renderer,CARDS[5],NULL,&card1);
        SDL_RenderCopy(renderer,CARDS[9],NULL,&card2);
        //render les dés
        SDL_RenderCopy(renderer,white_dice[0],NULL,&dice0);
        SDL_RenderCopy(renderer,white_dice[1],NULL,&dice1);
        SDL_RenderCopy(renderer,white_dice[2],NULL,&dice2);
        SDL_RenderCopy(renderer,white_dice[3],NULL,&dice3);
        SDL_RenderCopy(renderer,white_dice[4],NULL,&dice4);
        SDL_RenderCopy(renderer,white_dice[5],NULL,&dice5);
        //render le button de passer le tour
        SDL_RenderCopy(renderer,pass_turn,NULL,&pass_rect);
        //render le button de retourner les dés
        SDL_RenderCopy(renderer,return_dice,NULL,&return_rect);

        SDL_RenderPresent(renderer);
    }

    SDL_DestroyTexture(pass_turn);
    SDL_DestroyTexture(return_dice);

    for (int i = 0; i < 6; ++i) {
        SDL_DestroyTexture(white_dice[i]);
        SDL_DestroyTexture(red_dice[i]);
    }
    for (int i = 0; i < 15; ++i) {
        SDL_DestroyTexture(CARDS[i]);
    }
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    return 0;
}    