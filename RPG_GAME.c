#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

#include "MAP.h"
#include "CHARACTERS.h"
#include "BATTLE.h"
#include "CORE.h"

int main()
{
    // Seed random number generator
    srand(time(NULL));

    // Setup map
    char map[MAP_HEIGHT][MAP_WIDTH];
    Setup_Map(map);

    // Setup player
    Player player = Setup_Player();

    // Place player on map
    map[player.player_y][player.player_x] = '@';

    // Display map
    Display_Map(map);

    // Start battle
    bool won = battle(&player);
    if (won == false)
    {
        printf("You Lost!\n");
        // Restart game
    }

}