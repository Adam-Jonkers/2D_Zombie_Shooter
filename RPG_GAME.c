#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <ncurses.h>

#include "MAP.h"
#include "CHARACTERS.h"
#include "BATTLE.h"
#include "CORE.h"

int main(void)
{
    // Seed random number generator
    srand(time(NULL));

    // Setup display
    int max_y, max_x;
    // initilise_display(&max_y, &max_x);
    initscr();
    noecho();
    getmaxyx(stdscr, max_y, max_x);
    max_x -= 10;
    max_y -= 5;
    start_color();
    init_color(8, 294, 714, 937);
    init_color(9, 204, 549, 192);
    init_color(10, 0, 200, 0);
    init_color(11, 100 * 4, 100 * 4, 100 * 4);
    init_color(12, 100 * 5, 100 * 5, 100 * 5);
    init_color(13, 100 * 6, 100 * 6, 100 * 6);
    init_color(14, 100 * 7, 100 * 7, 100 * 7);
    init_color(15, 1000, 1000, 1000);
    init_pair(1, 8, 8);
    init_pair(2, 9, 9);
    init_pair(3, 10, 10);
    init_pair(4, 11, 11);
    init_pair(5, 12, 12);
    init_pair(6, 13, 13);
    init_pair(7, 14, 14);
    init_pair(8, 15, 15);

    // Setup player
    Player player = Setup_Player();

    // Setup map
    float noisemap[max_x][max_y];
    char map[max_x][max_y];
    Setup_Noise_Map(max_x, max_y, noisemap);
    Setup_Map(max_x, max_y, map, noisemap, &player);

    // Display map
    Display_Map(max_x, max_y, map);

    // Start battle
    bool won = battle(&player);
    if (won == false)
    {
        printf("You Lost!\n");
        // Restart game
    }
    endwin();
}