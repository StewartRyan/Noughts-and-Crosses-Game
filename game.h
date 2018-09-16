#ifndef ASSIGNMENT2_GAME_H
#define ASSIGNMENT2_GAME_H

#include <stdbool.h>

#define MAX_NAME_LEN 50


typedef enum { P1_TURN, P2_TURN, P1_WON, P2_WON, DRAW } status;

struct game {
    char board[3][3];
    char playerNames[2][MAX_NAME_LEN];
    int status;
    bool finished;
};

struct info {
    char x;
    int n;
};
void change(struct  info * pInfo);

void play_game(const char*, const char*);
void initialise_game(struct game*,const char*,const char*);

void draw_banner();
void display_board(char [3][3]);
void print_status(struct game *);
void display_board_positions();
void get_row_col(int, int*, int*);
void process_move(struct game *);
void update_status(struct game *);
bool check_if_draw(struct game *);
bool are_items_same(const char items[]);
bool find_horizontal_solutions(struct game *p_game_info);
bool find_vertical_solutions(struct game *p_game_info);
bool find_diagonal_solutions(struct game *p_game_info);
void randomiseStartingPlayer(struct game *);

int* allocateNums();
int addNums(const int* pList);

#endif //ASSIGNMENT2_GAME_H