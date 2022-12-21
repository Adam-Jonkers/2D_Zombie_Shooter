#include <stdio.h>
#include <stdbool.h>

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

void Move_Player(int map_width, int map_height, char map[map_height][map_width], Player* player)
{
    // Move player
    printf("Where do you want to move? \n1. Up \n2. Down \n3. Left \n4. Right\n\n");
    int c;
    while ((c = getchar()) != '\n' && c != EOF) 
    { 
        continue;
    }
    char action = getchar();
    printf("\nYou chose %c, \n", action);
    switch (action - '0')
    {
    case 1:
        if (map[player->player_y - 1][player->player_x] != '#') {
            map[player->player_y][player->player_x] = '.';
            player->player_y -= 1;
            map[player->player_y][player->player_x] = '@';
        }
        break;
    case 2:
        if (map[player->player_y + 1][player->player_x] != '#') {
            map[player->player_y][player->player_x] = '.';
            player->player_y += 1;
            map[player->player_y][player->player_x] = '@';
        }
        break;
    case 3:
        if (map[player->player_y][player->player_x - 1] != '#') {
            map[player->player_y][player->player_x] = '.';
            player->player_x -= 1;
            map[player->player_y][player->player_x] = '@';
        }
        break;
    case 4:
        if (map[player->player_y][player->player_x + 1] != '#') {
            map[player->player_y][player->player_x] = '.';
            player->player_x += 1;
            map[player->player_y][player->player_x] = '@';
        }
        break;
    default:
        printf("Invalid input\n");
        break;
    }
}