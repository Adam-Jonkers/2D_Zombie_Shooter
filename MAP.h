#ifndef MAP_H
#define MAP_H

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

void generate_texture_map(int max_x, int max_y, float* noisemap);

float fade(float t);

vec2 grad(vec2 p, float* noisemap, int width);

float noise(vec2 p, float* noisemap, int width);

void Setup_Noise_Map(int max_x, int max_y, float* noisemap, float* randarray);

void Setup_Map(int max_x, int max_y ,char* map, float* noisemap);

void Draw_Map(int max_x, int max_y, char* map, SDL_Renderer* renderer);

//void Move_Player(int max_x, int max_y, char map[max_x][max_y], Player* player, int* Gamestate, int* display_x, int* display_y, int displaysize_x, int displaysize_y);

#endif