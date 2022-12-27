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

#define PLAYER_PAIR 9

int main(void)
{
    int Gamestate = 1;
    /*
    1 = Moving
    2 = Battle
    3 = Shutdown
    */
    bool playing = true;

    // Seed random number generator
    srand(time(NULL));

    // Setup display
    int max_y = 600;
    int max_x = 600;
    int display_y;
    int display_x;
    int displaysize_y, displaysize_x;

    // initilise_display(&max_y, &max_x);
    initscr();
    noecho();
    cbreak();
    keypad(stdscr, TRUE);
    getmaxyx(stdscr, displaysize_y, displaysize_x);
    displaysize_x -= 10;
    displaysize_y -= 5;
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
    init_pair(PLAYER_PAIR, COLOR_BLACK, COLOR_RED);

    // Setup player
    Player player = Setup_Player();

    // Setup map
    player.player_x = get_random_number(displaysize_x / 2, max_x - displaysize_x / 2);
    player.player_y = get_random_number(displaysize_y / 2, max_y - displaysize_y / 2);

    display_x = player.player_x - displaysize_x / 2;
    display_y = player.player_y - displaysize_y / 2;
    float randarray[max_x][max_y];
    float noisemap[max_x][max_y];
    char map[max_x][max_y];
    Setup_Noise_Map(max_x, max_y, noisemap, randarray);
    Setup_Map(max_x, max_y, map, noisemap);

    // Display map
    Display_Map(max_x, max_y, map, &player, display_x, display_y, displaysize_x, displaysize_y);
    while (playing) {
        if (Gamestate == 1)
        {
            // Move player
            Move_Player(max_x, max_y, map, &player, &Gamestate, &display_x, &display_y, displaysize_x, displaysize_y);
        } else if (Gamestate == 2)
        {
            bool result = battle(&player);
            if (result) {
                Gamestate = 1;
                Display_Map(max_x, max_y, map, &player, display_x, display_y, displaysize_x, displaysize_y);
            } else {
                Gamestate = 3;
            }
        } else if (Gamestate == 3)
        {
            endwin();
            playing = false;
        }   
    }
}