#include <stdio.h>

#define MAP_WIDTH 80
#define MAP_HEIGHT 20

void Setup_Map(char map[MAP_HEIGHT][MAP_WIDTH])
{
    printf("Generating map...\n\n");
    // Setup map
    for (int y = 0; y < MAP_HEIGHT; y++) {
        for (int x = 0; x < MAP_WIDTH; x++) {
            if (y == 0 || y == MAP_HEIGHT - 1 || x == 0 || x == MAP_WIDTH - 1) {
                map[y][x] = '#';
            } else if (y % 2 == 0 && x % 2 == 0) {
                map[y][x] = 'O';
            } else {
                map[y][x] = '.';
            }
        }
    }
}

void Display_Map(char map[MAP_HEIGHT][MAP_WIDTH])
{
    // Display map
    for (int y = 0; y < MAP_HEIGHT; y++) {
        for (int x = 0; x < MAP_WIDTH; x++) {
            printf("%c", map[y][x]);
        }
        printf("\n");
    }
}