#ifndef MAP_H
#define MAP_H

#include "CHARACTERS.h"

void Setup_Map(int map_width, int map_height, char map[map_height][map_width], Player* player);

void Display_Map(int map_width, int map_height, char map[map_height][map_width]);

#endif