#ifndef MAP_H
#define MAP_H

#include <SDL2/SDL.h>

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

vec2_t grad(vec2_t p, float* noisemap, int width);

float noise(vec2_t p, float* noisemap, int width);

void Setup_Noise_Map(int max_x, int max_y, float* noisemap, float* randarray);

void Setup_Map_Png(bitmap_t map, float* noisemap);

SDL_Texture* Load_Map_Texture(SDL_Renderer* renderer);

void Draw_Map_Texture(SDL_Renderer* renderer, SDL_Texture* map_texture, int display_x, int display_y, int displaysize_x, int displaysize_y);

#endif