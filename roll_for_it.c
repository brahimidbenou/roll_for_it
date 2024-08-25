#include "roll_for_it.h"



void initialiser_carte(carte cards[NB_CARTES]){
    int i;
    for (i=0;i<3;i++)
    {
        cards[i].points = 15;
        cards[i].id = i;
        cards[i].taille_combinaison = 6;
        int r = i + 2;
        for (int k=0;k<cards[i].taille_combinaison ;k++){
            cards[i].combinaison[k] = r;
        }
    }
    for (i=3;i<7;i++)
    {
        cards[i].points = 10;
        cards[i].id = i;
        cards[i].taille_combinaison = 4;
        int r = i - 1;
        int r1 = i;
        for (int k=0;k<cards[i].taille_combinaison ;k++){
            if((k%2) == 0){
                cards[i].combinaison[k] = r;
            }
            else{
                cards[i].combinaison[k] = r1;
            }
        }
    }
    for (i=7;i<NB_CARTES;i++)
    {
        cards[i].points = 5;
        cards[i].id = i;
        cards[i].taille_combinaison = 3;
        
        for (int k=0;k<cards[i].taille_combinaison ;k++){
            int r = (k+i)%6 + 1;
            cards[i].combinaison[k] = r;
            
        }
    }
}


void initialiser_paquet(carte paquet_de_cartes[NB_CARTES])  //mélanger la carte 
{ 
    for (int i = 0; i < NB_CARTES; i++) 
    {
        int j = rand() % NB_CARTES;
        carte temp = paquet_de_cartes[i];
        paquet_de_cartes[i] = paquet_de_cartes[j];
        paquet_de_cartes[j] = temp;
    }
}


void initialiser_table(carte paquet_de_cartes[NB_CARTES], carte table[3],int * count)
{
    for (int i=0;i<3;i++)
    {
        table[i] = paquet_de_cartes[i];
        *count += 1;
    }
}


joueur creer_joueur(int numero) 
{
    joueur joueur;
    joueur.num = numero;
    
    for (int i = 0; i < NB_DES; i++) 
    {
        joueur.des[i][0] = i+1;
        joueur.des[i][1] = 0;
    }

    joueur.score = 0; //init du score
    return joueur;
}


void lancer_des(joueur * player) {
    for (int j = 0; j < NB_DES; j++) {
        if(player->des[j][1] == 0)
        {
            player->des[j][0] = (rand() % 6) + 1;
            printf("%d  ",player->des[j][0]);
            printf("%d \n",player->des[j][1]);
        } 
        else
        {
            printf("%d  ",player->des[j][0]);
            printf("%d \n",player->des[j][1]);
        }  
    }
    printf("\n");
}


void afficher_des(joueur * player){
    for (int i=0;i<NB_DES;i++)
    {
        printf("%d  ",player->des[i][0]);
        printf("%d \n",player->des[i][1]);
    }
}


void afficher_cartes(carte cartes[3]) 
{
    for (int i = 0; i < 3; i++) 
    {
        printf("Carte %d: %d points, Combinaison: ", cartes[i].id, cartes[i].points);
        for (int j = 0; j < cartes[i].taille_combinaison; j++) 
        {
            printf("%d ", cartes[i].combinaison[j]);
        }
        printf("\n");
    }
}


void place_dice(carte card[3], joueur * player) {
    int dice_index;
    int card_id;

    // Prompt for dice index
    printf("Choisir le de (0-%d): ", NB_DES - 1);
    scanf("%d", &dice_index);

    // Validate dice index
    if (dice_index < 0 || dice_index >= NB_DES) {
        printf("Index de dé invalide.\n");
        return;
    }

    // Verification que le des n'est pas déjà associé à une carte 
    if (player->des[dice_index][1] != 0) {
        printf("Le dé est déjà mis sur une carte\n");
        return;
    }

    // Prompt for card ID
    printf("Choisir la carte (ID de la carte): ");
    scanf("%d", &card_id);

    // Find the card by ID
    int card_index = -1;
    for (int k = 0; k < 3; k++) {
        if (card[k].id == card_id) {
            card_index = k;
            break;
        }
    }

    // Check if card was found
    if (card_index == -1) {
        printf("Carte non trouvée.\n");
        return;
    }

    // Check if dice matches any combination on the card
    int matched = 0;
    for (int j = 0; j < card[card_index].taille_combinaison; j++) {
        //printf("%d  %d\n",player->des[dice_index][0],card[card_index].combinaison[j]);
        if (player->des[dice_index][0] == card[card_index].combinaison[j]) {
            player->des[dice_index][1] = card[card_index].id;
            matched = 1;
            break;
        }
    }

    // If no match found
    if (!matched) {
        printf("Le dé ne correspond pas à la combinaison de la carte.\n");
    }
}


int verify(joueur ** player, carte card)
{
    int s = 0;
    for (int i=0;i<6;i++)
    {
        if ((*player)->des[i][1] == card.id)
        {
            s += 1;
        }
    }
    return s == card.taille_combinaison;
}


void take_card(joueur * player1,joueur * player2, carte table[3], carte paquet_de_cartes[NB_CARTES],int *count)
{
    int i;
    int v = 0;
    carte card1 ;
    for (i=0;i<3;i++){
        if(verify(&player1, table[i]))
        {
            card1 = table[i];
            v = 1;
            break;
        }
    }
    if (v){  
        player1->score += card1.points;
        return_card_dice_to_players(card1, &player1);
        return_card_dice_to_players(card1, &player2);
        table[i] = paquet_de_cartes[*count];
        *count += 1;
    }
}


void return_card_dice_to_players(carte card, joueur ** player1)
{
    for (int j=0; j<6; j++)
    {
        if ((*player1)->des[j][1] == card.id)
        {
            (*player1)->des[j][1] = 0;
        }
    }
}


void return_dice(joueur * player) {
    //(void)player;
    for (int i = 0; i < NB_DES; i++) {
        player->des[i][1] = 0;
    }
}


int Is_winner(joueur * player) 
{
    return (player->score >= 40);    
}
