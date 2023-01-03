#include <stdlib.h>
#include <math.h>
#include <SDL2/SDL.h>

#include "CORE.h"

int get_random_number(int min, int max)
{
    return (rand() % (max - min + 1) + min);
}

vec2 normalise(vec2 v) 
{
    float length = sqrt(v.x * v.x + v.y * v.y);
    return (vec2){v.x / length, v.y / length};
}

vec2 floor_vec2(vec2 v) 
{
    return (vec2){floor(v.x), floor(v.y)};
}

vec2 add_vec2(vec2 v1, vec2 v2) 
{
    return (vec2){v1.x + v2.x, v1.y + v2.y};
}

vec2 subtract_vec2(vec2 v1, vec2 v2) 
{
    return (vec2){v1.x - v2.x, v1.y - v2.y};
}

vec2 divide_vec2(vec2 v, float f) 
{
    return (vec2){v.x / f, v.y / f};
}

vec2 length_vec2(vec2 v) 
{
    return (vec2){sqrt(v.x * v.x + v.y * v.y)};
}

float dot_product(vec2 v1, vec2 v2) 
{
    return v1.x * v2.x + v1.y * v2.y;
}

SDL_Texture* load_texture(char* path, SDL_Renderer* renderer)
{
    SDL_Surface *surface = IMG_Load(path);
    SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);
    return texture;
}
