#include <stdio.h>

#include "MAP.h"
#include "CHARACTERS.h"
#include "CORE.h"

void Setup_Map(int map_width, int map_height, char map[map_height][map_width], Player* player)
{
    printf("Generating map...\n\n");
    // Setup map
    for (int y = 0; y < map_height; y++) {
        for (int x = 0; x < map_width; x++) {
            if (y == 0 || y == map_height - 1 || x == 0 || x == map_width - 1) {
                map[y][x] = '#';
            } else if (y % 2 == 0 && x % 2 == 0) {
                map[y][x] = 'O';
            } else {
                map[y][x] = '.';
            }
        }
    }
    // Place player on map
    player->player_x = get_random_number(1, map_width - 2);
    player->player_y = get_random_number(1, map_height - 2);
    map[player->player_y][player->player_x] = '@';
}

void Display_Map(int map_width, int map_height, char map[map_height][map_width])
{
    // Display map
    for (int y = 0; y < map_height; y++) {
        for (int x = 0; x < map_width; x++) {
            printf("%c", map[y][x]);
        }
        printf("\n");
    }
}