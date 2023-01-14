#ifndef MAP_H
#define MAP_H

#include <SDL2/SDL.h>

#include "CORE.h"
#include "PLAYER.h"

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

void generate_texture_map(int max_x, int max_y, float* noiseMap);

float fade(float t);

vec2_t grad(vec2_t p, float* noiseMap, int width);

float noise(vec2_t p, float* noiseMap, int width);

void Setup_Noise_Map(vec2_t max, float* noiseMap, float* randArray);

void Setup_Map_Png(bitmap_t map, float* noiseMap);

SDL_Texture* Load_Map_Texture(SDL_Renderer* renderer);

void Draw_Map_Texture(SDL_Renderer* renderer, SDL_Texture* map_texture, Player_t* player, vec2_t windowSize);

float Get_speed_multiplier(vec2_t position, vec2_t max, float* noiseMap);

#endif