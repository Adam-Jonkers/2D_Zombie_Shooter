#ifndef MAP_H
#define MAP_H

#include "CHARACTERS.h"
#include "CORE.h"

typedef struct {
    char description[100];
    int size;
    char map[100][100];
} Type;

typedef struct {
    Type type;
    char name[20];
    int x;
    int y;
} Location;

void generate_texture_map(int max_x, int max_y, float noisemap[max_x][max_y]);

float fade(float t);

vec2 grad(vec2 p, int max_x, int max_y, float noisemap[max_x][max_y]);

float noise(vec2 p, int max_x, int max_y, float noisemap[max_x][max_y]);

void Setup_Noise_Map(int max_x, int max_y, float noisemap[max_x][max_y]);

void Setup_Map(int max_x, int max_y ,char map[max_x][max_y], float noisemap[max_x][max_y], Player* player);

void Display_Map(int max_x, int max_y, char map[max_x][max_y], Player* player);

void Move_Player(int max_x, int max_y, char map[max_x][max_y], Player* player, int* Gamestate);

#endif