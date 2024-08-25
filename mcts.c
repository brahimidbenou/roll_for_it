#include "roll_for_it.h"


Node* create_node(GameState state, Node *parent) 
{
    Node *node = (Node *)malloc(sizeof(Node));
    if (node == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        exit(EXIT_FAILURE);
    }
    node->state = state;
    node->parent = parent;
    node->children = NULL;
    node->num_children = 0;
    node->visits = 0;
    node->value = 0.0;
    return node;
}

void free_node(Node *node) 
{
    for (int i = 0; i < node->num_children; i++) {
        free_node(node->children[i]);
    }
    free(node->children);
    free(node);
}

int is_game_over(GameState state)
{
    return ((state.player)->score >= 40) || ((state.player1)->score >= 40);
}

Node* select_best_child(Node *node) 
{
   
    Node *best_child = NULL;
    double best_value = -INFINITY;

    for (int i = 0; i < node->num_children; i++) {
        Node *child = node->children[i];
        double uct_value = (child->value / (child->visits + 1)) +
                           sqrt(2 * log(node->visits + 1) / (child->visits + 1));
        if (uct_value > best_value) {
            best_value = uct_value;
            best_child = child;
        }
    }
    return best_child;
}

void expand_node(Node *node) 
{
    GameState *possible_states = generate_possible_states(node->state);

    node->children = (Node **)malloc(POSSIBLE_STATES * sizeof(Node *));
    if (node->children == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        exit(EXIT_FAILURE);
    }
    node->num_children = POSSIBLE_STATES;

    for (int i = 0; i < POSSIBLE_STATES; i++) {
        node->children[i] = create_node(possible_states[i], node);
    }

    free(possible_states);
}

GameState simulate_random_game(GameState state)
{
    int i = 0;
    while (!is_game_over(state)) {
        GameState *possible_states = generate_possible_states(state);
        int random_index = rand() % POSSIBLE_STATES;
        state = possible_states[random_index];
        if (i == 260)
        {
            break;
        }
        i++;
        free(possible_states);
    }
    return state; 
}

void backpropagate(Node *node, GameState result) 
{
    double value = evaluate_result(result);

    while (node != NULL) {
        node->visits++;
        node->value += value;
        node = node->parent;
    }
}


GameState* generate_possible_states(GameState state) 
{
    int num_possible_moves = 1 << NB_DES; 

    GameState* possible_states = (GameState*)malloc(num_possible_moves * sizeof(GameState));
    int possible_state_index = 0;
    possible_states[0] = state;

    for (int i = 1; i < num_possible_moves; ++i) 
    {
        GameState new_state = state;

        int dice_index = 0;
        for (int j = 0; j < NB_DES; ++j) 
        {
            
            
                new_state.de[dice_index][0] = state.de[j][0];
                new_state.de[dice_index][1] = state.de[j][1];
                dice_index++;
            
        }
        // ce bloc doit etre modifier pour bien creer les etats possibles
        for (int c = 0; c < 3; ++c) 
        {
            for (int k = 0; k < state.cards[c].taille_combinaison; ++k) 
            {
                for (int d = 0; d < dice_index; ++d) 
                {
                    if (new_state.de[d][0] == state.cards[c].combinaison[k]) 
                    {
                        new_state.de[d][1] = state.cards[c].id;
                        break;
                    }
                }
            }
            if (verify(new_state.player, new_state.cards[c])) 
            {
                new_state.player->score += state.cards[c].points;
                new_state.cards[c].points = 0;
                new_state.count++;
            }
        }

        possible_states[possible_state_index++] = new_state;
    }

    return possible_states;
}

double evaluate_result(GameState result) 
{
    if(((result.player)->score) >= 40)
    {
        return 1.0; // ia a gagne
    }
    else
    {
        return 0.0; // ia a perdu
    }
}


GameState mcts(GameState game) 
{
    Node *root = create_node(game, NULL);
    //int iterations = 100;
    for (int i = 0; i < 100; i++) 
    {
        Node *node = root;
        while (node->num_children > 0) 
        {
            node = select_best_child(node);
        }
        if (!is_game_over(node->state)) 
        {
            expand_node(node);
        }
        GameState result = simulate_random_game(node->state);
    
        backpropagate(node, result);
        
    }
    Node *best_child = select_best_child(root);
    GameState next_state = best_child->state;
    free_node(root);

    return next_state;
}