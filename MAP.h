#ifndef MAP_H
#define MAP_H

#include "CHARACTERS.h"
#include "CORE.h"

#define MAP_WIDTH 600
#define MAP_HEIGHT 600

#define LAKE_COLOR 8
#define PLAINS_COLOR 9
#define FOREST_COLOR 10
#define MOUNTAIN_1_COLOR 11
#define MOUNTAIN_2_COLOR 12
#define MOUNTAIN_3_COLOR 13
#define MOUNTAIN_4_COLOR 14
#define SNOW_COLOR 15

#define LAKE_PAIR 1
#define PLAINS_PAIR 2
#define FOREST_PAIR 3
#define MOUNTAIN_1_PAIR 4
#define MOUNTAIN_2_PAIR 5
#define MOUNTAIN_3_PAIR 6
#define MOUNTAIN_4_PAIR 7
#define SNOW_PAIR 8


#define PLAYER_PAIR 9
#define ERROR_PAIR 10

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

void Setup_Noise_Map(int max_x, int max_y, float noisemap[max_x][max_y], float randarray[max_x][max_y]);

void Setup_Map(int max_x, int max_y ,char map[max_x][max_y], float noisemap[max_x][max_y]);

void Display_Map(int max_x, int max_y, char map[max_x][max_y], SDL_Renderer* renderer);

void Move_Player(int max_x, int max_y, char map[max_x][max_y], Player* player, int* Gamestate, int* display_x, int* display_y, int displaysize_x, int displaysize_y);

#endif