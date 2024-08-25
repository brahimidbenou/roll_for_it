#ifndef ROLL_FOR_IT_H
#define ROLL_FOR_IT_H

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include "time.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#define NB_JOUEUR 2
#define NB_DES 6
#define NB_CARTES 15  
#define WINDOW_WIDTH 900
#define WINDOW_HEIGHT 600


typedef struct {
    int num; //peut étre 1 ou 2
    int score; //le score obtenu
    int des[NB_DES][2]; //son résultat lors du lancé du dés;
} joueur;

typedef struct {
    int points; 
    int id;
    int taille_combinaison;
    int combinaison[6]; //6 places au max 
} carte;

//fonction du jeu roll_for_it.c
void initialiser_carte(carte cards[NB_CARTES]);
void initialiser_paquet(carte paquet_de_cartes[NB_CARTES]);
void afficher_cartes(carte cartes[3]);
void initialiser_table(carte paquet_de_cartes[NB_CARTES], carte table[3], int *count);
joueur creer_joueur(int numero);
void lancer_des(joueur * player);
void afficher_des(joueur * player);
void place_dice(carte card[3], joueur * player);
int verify(joueur ** player, carte card);
void take_card(joueur * player1,joueur * player2, carte table[3], carte paquet_de_cartes[NB_CARTES],int *count);
void return_card_dice_to_players(carte card, joueur ** player1);
void return_dice(joueur * player);
int Is_winner(joueur * player);

//fonction de l'interface graphique SDL.c
SDL_Texture* load_texture_from_image(const char* file_image_name, SDL_Window* window, SDL_Renderer* renderer);
void end_sdl(char ok, const char* msg, SDL_Window* window, SDL_Renderer* renderer);
int afficherFenetreDemarrage(SDL_Renderer *renderer, SDL_Window* window);
int afficherSelectionMode(SDL_Renderer * renderer);
int EndGame(SDL_Renderer *renderer, SDL_Window* window, int win);



#endif
