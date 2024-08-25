#include "roll_for_it.h"
#include <SDL2/SDL_image.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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
                    continuer = SDL_FALSE;
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

int choose_dice(int x_dice, int y_dice){
    int dice_index = -1; // Initialize to -1 for invalid index

    // Define dice areas and check if the click is within any of them
    SDL_Rect dice_areas[6] = {
        {120, 437, DICE_SIZE, DICE_SIZE},
        {237, 437, DICE_SIZE, DICE_SIZE},
        {354, 437, DICE_SIZE, DICE_SIZE},
        {471, 437, DICE_SIZE, DICE_SIZE},
        {588, 437, DICE_SIZE, DICE_SIZE},
        {705, 437, DICE_SIZE, DICE_SIZE}
    };

    for (int i = 0; i < 6; ++i) {
        if (x_dice > dice_areas[i].x && x_dice < dice_areas[i].x + DICE_SIZE &&
            y_dice > dice_areas[i].y && y_dice < dice_areas[i].y + DICE_SIZE) {
            dice_index = i;
            break;
        }
    }

    if (x_dice > 800 && x_dice < 900 && y_dice > 520 && y_dice < 600) {
        dice_index = 6; // Pass turn button
    }

    return dice_index;
}

int choose_card(int x_card, int y_card) {
    int card_index = -1; // Initialize to -1 for invalid index

    if (x_card > 100 && x_card < 300 && y_card > 75 && y_card < 425) {
        card_index = 0;
    } else if (x_card > 350 && x_card < 550 && y_card > 75 && y_card < 425) {
        card_index = 1;
    } else if (x_card > 600 && x_card < 800 && y_card > 75 && y_card < 425) {
        card_index = 2;
    }

    return card_index;
}

void place_dice_sdl(carte card[3], joueur *player, int dice_index, int card_index) {
    if (player->des[dice_index][1] != 0) {
        printf("Le dé est déjà mis sur une carte\n"); // To be replaced with textures
        return;
    }

    int matched = 0;
    for (int j = 0; j < card[card_index].taille_combinaison; j++) {
        if (player->des[dice_index][0] == card[card_index].combinaison[j]) {
            player->des[dice_index][1] = card[card_index].id;
            matched = 1;
            break;
        }
    }

    if (!matched) {
        printf("Le dé ne correspond pas à la combinaison de la carte.\n"); // To be replaced with textures
    }
}

void draw_dice(SDL_Renderer *renderer, SDL_Texture *textures[], SDL_Rect rect[6], joueur *player) {
    for (int i = 0; i < NB_DES; i++) {
        if (player->des[i][1] == 0) {
            SDL_RenderCopy(renderer, textures[player->des[i][0] - 1], NULL, &rect[i]);
        }
    }
}

int main() {
    SDL_Init(SDL_INIT_VIDEO);
    IMG_Init(IMG_INIT_JPG | IMG_INIT_PNG);

    SDL_Window *window = SDL_CreateWindow("Roll For It!", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_SHOWN);
    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    //afficherFenetreDemarrage(renderer, window);
    
    SDL_Texture* red_dice[] = {
        load_texture_from_image("SDL_image/dice/R1.jpg", window, renderer),
        load_texture_from_image("SDL_image/dice/R2.jpg", window, renderer),
        load_texture_from_image("SDL_image/dice/R3.jpg", window, renderer),
        load_texture_from_image("SDL_image/dice/R4.jpg", window, renderer),
        load_texture_from_image("SDL_image/dice/R5.jpg", window, renderer),
        load_texture_from_image("SDL_image/dice/R6.jpg", window, renderer)
    };
    
    SDL_Texture* white_dice[] = {
        load_texture_from_image("SDL_image/dice/W1.jpg", window, renderer),
        load_texture_from_image("SDL_image/dice/W2.jpg", window, renderer),
        load_texture_from_image("SDL_image/dice/W3.jpg", window, renderer),
        load_texture_from_image("SDL_image/dice/W4.jpg", window, renderer),
        load_texture_from_image("SDL_image/dice/W5.jpg", window, renderer),
        load_texture_from_image("SDL_image/dice/W6.jpg", window, renderer)
    };
    
    SDL_Texture* CARDS[] = {
        load_texture_from_image("SDL_image/cards/card15/0.png", window, renderer),
        load_texture_from_image("SDL_image/cards/card15/1.png", window, renderer),
        load_texture_from_image("SDL_image/cards/card15/2.png", window, renderer),
        load_texture_from_image("SDL_image/cards/card10/3.png", window, renderer),
        load_texture_from_image("SDL_image/cards/card10/4.png", window, renderer),
        load_texture_from_image("SDL_image/cards/card10/5.png", window, renderer),
        load_texture_from_image("SDL_image/cards/card10/6.png", window, renderer),
        load_texture_from_image("SDL_image/cards/card5/7.png", window, renderer),
        load_texture_from_image("SDL_image/cards/card5/8.png", window, renderer),
        load_texture_from_image("SDL_image/cards/card5/9.png", window, renderer),
        load_texture_from_image("SDL_image/cards/card5/10.png", window, renderer),
        load_texture_from_image("SDL_image/cards/card5/11.png", window, renderer),
        load_texture_from_image("SDL_image/cards/card5/12.png", window, renderer),
        load_texture_from_image("SDL_image/cards/card5/13.png", window, renderer),
        load_texture_from_image("SDL_image/cards/card5/14.png", window, renderer)
    };

    SDL_Texture* roll_dice = load_texture_from_image("SDL_image/roll.png", window, renderer);
    SDL_Texture* pass_turn = load_texture_from_image("SDL_image/pass_turn.png", window, renderer);
    SDL_Texture* return_dice = load_texture_from_image("SDL_image/return.jpg", window, renderer);

    // Initialize cards
    carte cards[NB_CARTES];
    initialiser_carte(cards);
    
    // Initialize players
    joueur joueur_W = creer_joueur(1);
    joueur joueur_R = creer_joueur(2);
    
    // Initialize the table
    int count = 0;
    carte table[3];
    initialiser_table(cards, table, &count);
    
    int running = 1;
    SDL_Event event;
    while (running) {
        while (SDL_PollEvent(&event)) {            
            if (event.type == SDL_QUIT) {
                running = 0;
            } else if (event.type == SDL_KEYDOWN) {
                if (event.key.keysym.sym == SDLK_SPACE) {
                    lancer_des(&joueur_W);
                }
            if (event.type == SDL_MOUSEBUTTONDOWN) {
                int dice_index = -1;
                int card_index = -1;
                int x, y;
                SDL_GetMouseState(&x, &y);
                printf("%d %d\n", x ,y);
                dice_index = choose_dice(x, y);
                if (dice_index != -1 && dice_index < 6) {
                    SDL_GetMouseState(&x, &y);
                    card_index = choose_card(x, y);
                    if (card_index != -1) {
                        place_dice_sdl(table, &joueur_W, dice_index, card_index);
                    }
                }
            }
        }
        }

        SDL_SetRenderDrawColor(renderer, 255, 255, 255, SDL_ALPHA_OPAQUE); // White background
        SDL_RenderClear(renderer);

        SDL_Rect card0 = {100, 75, 200, 350};
        SDL_Rect card1 = {350, 75, 200, 350};
        SDL_Rect card2 = {600, 75, 200, 350};

        SDL_Rect dice_rect[] = {
            {120, 437, 75, 75},
            {237, 437, 75, 75},
            {354, 437, 75, 75},
            {471, 437, 75, 75},
            {588, 437, 75, 75},
            {705, 437, 75, 75}
        };

        SDL_Rect roll_rect = {400, 520, 100, 80};
        SDL_Rect return_rect = {0, 520, 100, 80};
        SDL_Rect pass_rect = {800, 520, 100, 80};

        // Render cards on the table
        SDL_RenderCopy(renderer, CARDS[0], NULL, &card0);
        SDL_RenderCopy(renderer, CARDS[5], NULL, &card1);
        SDL_RenderCopy(renderer, CARDS[9], NULL, &card2);
        
        // Render dice
        draw_dice(renderer, white_dice, dice_rect, &joueur_W);
        
        // Render buttons
        SDL_RenderCopy(renderer, pass_turn, NULL, &pass_rect);
        SDL_RenderCopy(renderer, return_dice, NULL, &return_rect);
        SDL_RenderCopy(renderer, roll_dice, NULL, &roll_rect);

        SDL_RenderPresent(renderer);
    }

    SDL_DestroyTexture(pass_turn);
    SDL_DestroyTexture(return_dice);
    SDL_DestroyTexture(roll_dice);
    
    for (int i = 0; i < 6; ++i) {
        SDL_DestroyTexture(white_dice[i]);
        SDL_DestroyTexture(red_dice[i]);
    }
    for (int i = 0; i < 15; ++i) {
        SDL_DestroyTexture(CARDS[i]);
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    IMG_Quit();
    SDL_Quit();
    return 0;
}
