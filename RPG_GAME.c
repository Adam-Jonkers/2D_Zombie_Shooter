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
    int max_y = MAP_HEIGHT;
    int max_x = MAP_WIDTH;
    int display_y;
    int display_x;
    int displaysize_y, displaysize_x;

    // initilise_display(&max_y, &max_x);
    initscr();
    noecho();
    cbreak();
    curs_set(0);
    keypad(stdscr, TRUE);
    getmaxyx(stdscr, displaysize_y, displaysize_x);
    displaysize_x -= 10;
    displaysize_y -= 5;
    start_color();
    init_color(LAKE_COLOR, 294, 714, 937);
    init_color(PLAINS_COLOR, 204, 549, 192);
    init_color(FOREST_COLOR, 0, 200, 0);
    init_color(MOUNTAIN_1_COLOR, 400, 400, 400);
    init_color(MOUNTAIN_2_COLOR, 500, 500, 500);
    init_color(MOUNTAIN_3_COLOR, 600, 600, 600);
    init_color(MOUNTAIN_4_COLOR, 700, 700, 700);
    init_color(SNOW_COLOR, 1000, 1000, 1000);
    init_pair(LAKE_PAIR, LAKE_COLOR, LAKE_COLOR);
    init_pair(PLAINS_PAIR, PLAINS_COLOR, PLAINS_COLOR);
    init_pair(FOREST_PAIR, FOREST_COLOR, FOREST_COLOR);
    init_pair(MOUNTAIN_1_PAIR, MOUNTAIN_1_COLOR, MOUNTAIN_1_COLOR);
    init_pair(MOUNTAIN_2_PAIR, MOUNTAIN_2_COLOR, MOUNTAIN_2_COLOR);
    init_pair(MOUNTAIN_3_PAIR, MOUNTAIN_3_COLOR, MOUNTAIN_3_COLOR);
    init_pair(MOUNTAIN_4_PAIR, MOUNTAIN_4_COLOR, MOUNTAIN_4_COLOR);
    init_pair(SNOW_PAIR, SNOW_COLOR, SNOW_COLOR);
    init_pair(PLAYER_PAIR, COLOR_BLACK, COLOR_RED);
    init_pair(ERROR_PAIR, COLOR_BLACK, COLOR_RED);

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
    Generate_River(max_x, max_y, map, noisemap);

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