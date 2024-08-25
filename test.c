#include "roll_for_it.h"

void initialiser_carte(carte cards[NB_CARTES]) {
    int i;
    for (i = 0; i < 3; i++) {
        cards[i].points = 15;
        cards[i].id = i;
        cards[i].taille_combinaison = 6;
        int r = i + 2;
        for (int k = 0; k < cards[i].taille_combinaison; k++) {
            cards[i].combinaison[k] = r;
        }
    }
    for (i = 3; i < 7; i++) {
        cards[i].points = 10;
        cards[i].id = i;
        cards[i].taille_combinaison = 4;
        int r = i - 1;
        int r1 = i;
        for (int k = 0; k < cards[i].taille_combinaison; k++) {
            cards[i].combinaison[k] = (k % 2 == 0) ? r : r1;
        }
    }
    for (i = 7; i < NB_CARTES; i++) {
        cards[i].points = 5;
        cards[i].id = i;
        cards[i].taille_combinaison = 3;
        int r = i - 4;
        for (int k = 0; k < cards[i].taille_combinaison; k++) {
            cards[i].combinaison[k] = r;
        }
    }
}

void afficher_cartes(carte cartes[3]) {
    for (int i = 0; i < 3; i++) {
        printf("Carte %d: Points: %d, Combinaison: ", cartes[i].id, cartes[i].points);
        for (int j = 0; j < cartes[i].taille_combinaison; j++) {
            printf("%d ", cartes[i].combinaison[j]);
        }
        printf("\n");
    }
}

joueur creer_joueur(int numero) {
    joueur player;
    player.num = numero;
    player.score = 0;
    for (int i = 0; i < NB_DES; i++) {
        player.des[i][0] = 0;
        player.des[i][1] = 0;
    }
    return player;
}

void initialiser_paquet(carte paquet_de_cartes[NB_CARTES]) {
    initialiser_carte(paquet_de_cartes);
}

void initialiser_table(carte paquet_de_cartes[NB_CARTES], carte table[3], int *count) {
    for (int i = 0; i < 3; i++) {
        table[i] = paquet_de_cartes[*count];
        (*count)++;
    }
}

void place_Cards(carte table[3], joueur player) {
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < NB_DES; j++) {
            if (player.des[j][0] == table[i].combinaison[j]) {
                player.des[j][1] = table[i].id;
            }
        }
    }
}

int verify(joueur player, carte card) {
    for (int i = 0; i < card.taille_combinaison; i++) {
        int found = 0;
        for (int j = 0; j < NB_DES; j++) {
            if (player.des[j][0] == card.combinaison[i] && player.des[j][1] == card.id) {
                found = 1;
                break;
            }
        }
        if (!found) {
            return 0;
        }
    }
    return 1;
}

void take_card(joueur player, carte table[3], carte paquet_de_cartes[NB_CARTES], joueur players[NB_JOUEUR], int *count) {
    for (int i = 0; i < 3; i++) {
        if (verify(player, table[i])) {
            player.score += table[i].points;
            return_Cards_to_players(table[i], players);
            table[i] = paquet_de_cartes[*count];
            (*count)++;
            break;
        }
    }
}

void return_Cards_to_players(carte card, joueur players[NB_JOUEUR]) {
    for (int i = 0; i < NB_JOUEUR; i++) {
        joueur *player = &players[i];
        for (int j = 0; j < NB_DES; j++) {
            if (player->des[j][1] == card.id) {
                player->des[j][1] = 0;
            }
        }
    }
}

void return_Cards(joueur player) {
    for (int j = 0; j < NB_DES; j++) {
        player.des[j][1] = 0;
    }
}

void lancer_des(joueur player) {
    for (int j = 0; j < NB_DES; j++) {
        player.des[j][0] = (rand() % 6) + 1;
        player.des[j][1] = 0;
    }
}

int Is_winner(joueur player) {
    return player.score >= 40;
}




