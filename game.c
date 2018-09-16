//
// Created by Ryan on 24/03/2018.
//
#include <stdio.h>
#include <stdlib.h>
#include <memory.h>
#include <time.h>
#include "game.h"

// Constants
const char SPACE        = ' ';
const char X_SYMBOL     = 'X';
const char O_SYMBOL     = 'O';
const bool RANDOM_START = true;

/**
 * Starts the game
 */
void play_game(const char* name1, const char* name2) {
    // Title
    printf("Xs and Os!\n");

    // Create a struct instance
    struct game* p_game_info;

    // Allocate some memory to the struct
    p_game_info = malloc(sizeof(struct game));

    // Initialise the struct
    initialise_game(p_game_info, name1, name2);

    // Game loop
    while (!p_game_info->finished) {
        draw_banner();
        display_board_positions();
        display_board(p_game_info->board);
        print_status(p_game_info);
        process_move(p_game_info);
    }

    // Display result of the game
    display_board(p_game_info->board);
    print_status(p_game_info);
}

/**
 * Initialises an instance of the game structure
 * @param p_game_info
 * @param name1
 * @param name2
 */
void initialise_game(struct game *p_game_info, const char *name1, const char *name2) {
    // Set every space in the game grid to SPACE ("-")
    for (int i = 0; i < 3 ; ++i)
        for (int j = 0; j < 3; ++j)
            p_game_info->board[i][j] = SPACE;

    // Set both names
    strncpy(p_game_info->playerNames[0], name1, MAX_NAME_LEN);
    strncpy(p_game_info->playerNames[1], name2, MAX_NAME_LEN);

    // Set is finished boolean
    p_game_info->finished = false;

    // Set status
   // p_game_info->status = P1_TURN;
    randomiseStartingPlayer(p_game_info);
}

/**
 * Randomises the starting player
 * @param p_game_info
 */
void randomiseStartingPlayer(struct game *p_game_info) {
    if (RANDOM_START) {
        srand(time(0));
        p_game_info->status = ((rand() % 2) == 0) ? P1_TURN : P2_TURN;
    } else {
        printf("User not selected randomly.");
        p_game_info->status = P1_TURN;
    }
}

/**
 * Prints the banner for the game
 */
void draw_banner() {
//  for (int i = 0; i < 15; ++i) printf("\n");

    printf("-----------------\n|  GAME STATUS  |\n-----------------");
}

/**
 * Takes in a board 3x3 and displays it
 * @param board
 */
void display_board(char (*board)[3]) {
    for (int i = 0; i < 3 ; ++i) {
        // Print line separator only if it isn't the first row
        printf("\n   %s\n", (i==0) ? "" : "---------");

        // Print cell separator only if it isn't first cell
        for (int j = 0; j < 3; ++j)
            printf(" %c %c", (j==0) ? ' ' : '|', board[i][j]);
    }

    printf("\n\n");
}

/**
 * Prints status of the game
 * @param p_game_info
 */
void print_status(struct game *p_game_info) {

    // Check the status of the current game
    switch (p_game_info->status) {
        case P1_TURN: printf("%s's turn.\n", p_game_info->playerNames[0]); break;
        case P2_TURN: printf("%s's turn.\n", p_game_info->playerNames[1]); break;
        case P1_WON : printf("Well done %s you won\n", p_game_info->playerNames[0]); break;
        case P2_WON : printf("Well done %s you won\n", p_game_info->playerNames[1]); break;
        case DRAW   : printf("It's a draw\n"); break;
        default: break;
    }
}

/**
 * Display the positions on the board that a player can select
 */
void display_board_positions() {
    int counter = 0;
    for (int i = 0; i < 3 ; ++i) {
        // Print line separator only if it isn't the first row
        printf("\n   %s\n", (i==0) ? "" : "---------");

        // Print cell separator only if it isn't first cell
        for (int j = 0; j < 3; ++j) {
            printf(" %c %d", (j == 0) ? ' ' : '|', counter);
            counter++;
        }
    }
}

/**
 * Calculates the row and column the chosen position is located
 * @param position
 * @param row
 * @param col
 */
void get_row_col(int position, int* row, int* col) {
    position -= 48;

    *row = position/3;
    *col = position%3;
}

/**
 * Processes the player's move
 * @param p_game_info
 */
void process_move(struct game *p_game_info) {
    bool validTurn = false;

    // Keep going until a valid turn has been played
    while(!validTurn) {
        // Print input request
        printf("\nEnter your move 0-8: ");

        // Get input from user in type char
        int chosenPosition = getchar();
        getchar();

        // Check if ASCII char is within correct range for numbers 0-8
        if (chosenPosition >= 48 && chosenPosition <= 56) {
            // Declare row & col ints
            int row, col;

            // Get row and column values from position
            get_row_col(chosenPosition, &row, &col);

            // If position has not yet been played
            if (p_game_info->board[row][col] == SPACE) {
                // Get appropriate symbol for current player
                char currentSymbol = (p_game_info->status == P1_TURN) ? X_SYMBOL : O_SYMBOL;

                // Set slot as current player's symbol
                p_game_info->board[row][col] = currentSymbol;

                // Toggle status
                p_game_info->status = (p_game_info->status == P1_TURN) ? P2_TURN : P1_TURN;

                // Update status
                update_status(p_game_info);

                // Set valid turn to true
                validTurn = true;
            } else {
                printf("Slot has already been played\n");
            }
        }
    }
}

/**
 * Toggle between player X and player O and find winners after each turn
 * @param p_game_info
 */
void update_status(struct game *p_game_info) {
    // Find winners in all possible combinations
    if (check_if_draw(p_game_info)              ||
        find_diagonal_solutions(p_game_info)    ||
        find_horizontal_solutions(p_game_info)  ||
        find_vertical_solutions(p_game_info))
    {
        // Set status to DRAW if game is draw, set to winner if there is a winner
        p_game_info->status = (check_if_draw(p_game_info)) ? DRAW : ((p_game_info->status == P1_TURN) ? P2_WON : P1_WON);

        // Tell game that it's finished
        p_game_info->finished = true;
    }
}

/**
 * Checks if all items in given array are the exact same
 * @param items
 * @return
 */
bool are_items_same(const char *items) {
    // Checks if 3 array elements match and is not of type space
    if (items[0] == items[1] && items[1] == items[2] && items[0] != SPACE)
        return true;

    return false;
}

/**
 * Finds 3 in a row in all horizontal combinations
 * @param p_game_info
 * @return
 */
bool find_horizontal_solutions(struct game *p_game_info) {
    // Check if any horizontal matches are found
    for (int i = 0; i < 3; ++i)
        if (are_items_same(p_game_info->board[i]))
            return true;

    return false;
}

/**
 * Finds 3 in a row in all vertical combinations
 * @param p_game_info
 * @return
 */
bool find_vertical_solutions(struct game *p_game_info) {
    // Temporary array of board columns
    char columns[3][3];

    // Put columns into temp array
    for (int i = 0; i < 3; ++i)
        for (int j = 0; j < 3; ++j)
            columns[j][i] = p_game_info->board[i][j];

    // Check if any columns are complete matches
    for (int k = 0; k < 3; ++k)
        if (are_items_same(columns[k]))
            return true;

    // Return false, no matches were found
    return false;
}

/**
 * Finds 3 in a row in all diagonal combinations
 * @param p_game_info
 * @return
 */
bool find_diagonal_solutions(struct game *p_game_info) {
    char columns[2][3];
    int x = 2;

    for (int j = 0; j < 3; ++j) {
        columns[0][j] = p_game_info->board[j][j];
        columns[1][j] = p_game_info->board[j][x];
        x--;
    }

    // Check if any columns are complete matches
    for (int k = 0; k < 2; ++k)
        if (are_items_same(columns[k]))
            return true;
}

/**
 * Check if the current scenario in the game is a draw
 * @param p_game_info
 * @param moves
 * @return
 */
bool check_if_draw(struct game *p_game_info) {
    int moves = 0;

    // Put columns into temp array
    for (int i = 0; i < 3; ++i)
        for (int j = 0; j < 3; ++j)
             if (p_game_info->board[i][j] != SPACE)
                 moves++;

    // If both players haven't won after 9 total moves, it's a draw
    if (p_game_info->status != P1_WON && p_game_info->status != P2_WON && moves == 9)
        return true;

    return false;
}