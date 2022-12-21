#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include "MAP.h"
#include "CHARACTERS.h"
#include "BATTLE.h"
#include "CORE.h"

#define MAP_WIDTH 80
#define MAP_HEIGHT 20

int main(void)
{
    // Seed random number generator
    srand(time(NULL));

    // Setup player
    Player player = Setup_Player();

    // Setup map
    char map[MAP_HEIGHT][MAP_WIDTH];
    Setup_Map(MAP_WIDTH, MAP_HEIGHT, map, &player);

    // Display map
    Display_Map(MAP_WIDTH, MAP_HEIGHT, map);

    // Start battle
    bool won = battle(&player);
    if (won == false)
    {
        printf("You Lost!\n");
        // Restart game
    }

}