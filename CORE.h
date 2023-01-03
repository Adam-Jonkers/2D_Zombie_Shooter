#ifndef CORE_H
#define CORE_H

#include <SDL2/SDL.h>

typedef struct {
    float x;
    float y;
} vec2;

int get_random_number(int min, int max);

vec2 normalise(vec2 v);

vec2 floor_vec2(vec2 v);

vec2 add_vec2(vec2 v1, vec2 v2);

vec2 subtract_vec2(vec2 v1, vec2 v2);

vec2 divide_vec2(vec2 v, float f);

vec2 length_vec2(vec2 v);

float dot_product(vec2 v1, vec2 v2);

SDL_Texture* load_texture(char* path, SDL_Renderer* renderer);

#endif