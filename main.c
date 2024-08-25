#include "roll_for_it.h"

int main() {
    carte cards[NB_CARTES];
    srand(time(NULL));
    initialiser_carte(cards);
    joueur joueur_1 = creer_joueur(1);
    joueur joueur_2 = creer_joueur(2);
    initialiser_paquet(cards);
    int count = 0;
    carte table[3];
    initialiser_table(cards, table, &count);

    while (1) {
        int placer_des1;
        afficher_cartes(table);
        int b;
        printf("C'est le tour du joueur 1:\n");
        printf("Voulez-vous retourner vos dés? Si oui, tapez 1, sinon tapez 0\n");
        if (scanf("%d", &b) != 1) {
            fprintf(stderr, "Erreur de lecture\n");
            exit(EXIT_FAILURE);
        }
        if (b == 1) {
            return_dice(&joueur_1);
        }
        lancer_des(&joueur_1);
        printf("Voulez-vous placer un dé? Si oui, tapez 1, sinon tapez 0\n");
        if (scanf("%d", &placer_des1) != 1) {
            fprintf(stderr, "Erreur de lecture\n");
            exit(EXIT_FAILURE);
        }
        while (placer_des1) {
            place_dice(table, &joueur_1);
            afficher_cartes(table);
            afficher_des(&joueur_1);
            take_card(&joueur_1, &joueur_2, table, cards, &count);
            if (Is_winner(&joueur_1)) {
                printf("Le joueur 1 a gagné !!!!!!!\n");
                break;
            }
            printf("Voulez-vous placer un autre dé? Si oui, tapez 1, sinon tapez 0\n");
            if (scanf("%d", &placer_des1) != 1) {
                fprintf(stderr, "Erreur de lecture\n");
                exit(EXIT_FAILURE);
            }
        }
        if (Is_winner(&joueur_1)) {
            break;
        }

        int placer_des2;
        afficher_cartes(table);
        int b1;
        printf("C'est le tour du joueur 2:\n");
        printf("Voulez-vous retourner vos dés? Si oui, tapez 1, sinon tapez 0\n");
        if (scanf("%d", &b1) != 1) {
            fprintf(stderr, "Erreur de lecture\n");
            exit(EXIT_FAILURE);
        }
        if (b1 == 1) {
            return_dice(&joueur_2);
        }
        lancer_des(&joueur_2);
        printf("Voulez-vous placer un dé? Si oui, tapez 1, sinon tapez 0\n");
        if (scanf("%d", &placer_des2) != 1) {
            fprintf(stderr, "Erreur de lecture\n");
            exit(EXIT_FAILURE);
        }
        while (placer_des2) {
            //créer une copie de l'état de jeu pour l'IA
            GameState state;
            memcpy(state.paquet_de_cartes, cards, sizeof(cards));
            memcpy(state.cards, table, sizeof(table));
            state.count = count;
            state.score_ = joueur_2.score;
            memcpy(state.de, joueur_2.des, sizeof(joueur_2.des));
            state.player = &joueur_2;
            state.player1 = &joueur_1;
            
            for (int i=0; i<6; i++)
            {
                printf("%d ",state.de[i][0]);
                printf("%d \n",state.de[i][1]);
            }
            GameState new_state = mcts(state);
            //afficher_des(new_state.player);
            printf("%d %d\n", new_state.player->score, new_state.score_);
            //appliquer les changements de l'IA à l'état de jeu réel
            memcpy(table, new_state.cards, sizeof(table));
            memcpy(joueur_2.des, new_state.de, sizeof(joueur_2.des));
            joueur_2.score = new_state.score_;
            count = new_state.count;

            afficher_des(&joueur_2);
            take_card(&joueur_2, &joueur_1, table, cards, &count);
            if (Is_winner(&joueur_2)) {
                printf("Le joueur 2 a gagné !!!!!!!\n");
                break;
            }
            printf("Voulez-vous placer un autre dé? Si oui, tapez 1, sinon tapez 0\n");
            if (scanf("%d", &placer_des2) != 1) {
                fprintf(stderr, "Erreur de lecture\n");
                exit(EXIT_FAILURE);
            }
        }
        if (Is_winner(&joueur_2)) {
            break;
        }
    }
    return 0;
}