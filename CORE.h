#ifndef CORE_H
#define CORE_H

#include <SDL2/SDL.h>

typedef struct {
    float x;
    float y;
} vec2_t;

int get_random_number(int min, int max);

vec2_t normalise(vec2_t v);

vec2_t floor_vec2(vec2_t v);

vec2_t add_vec2(vec2_t v1, vec2_t v2);

vec2_t subtract_vec2(vec2_t v1, vec2_t v2);

vec2_t divide_vec2(vec2_t v, float f);

vec2_t length_vec2(vec2_t v);

float dot_product(vec2_t v1, vec2_t v2);

SDL_Texture* load_texture(char* path, SDL_Renderer* renderer);

#endif