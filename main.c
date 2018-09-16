#include <printf.h>
#include <stdlib.h>
#include <memory.h>
#include "game.h"

int main(int argc, char* argv[])
{
    if (argc == 3)
        play_game(argv[1], argv[2]);
    else
        printf("Error: You supplied the incorrect number of names.\n");


    return 0;
}