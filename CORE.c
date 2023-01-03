#include <stdlib.h>
#include <math.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include "CORE.h"

int get_random_number(int min, int max)
{
    return (rand() % (max - min + 1) + min);
}

vec2_t normalise_vec2(vec2_t v) 
{
    float length = length_vec2(v); 
    return (vec2_t){v.x / length, v.y / length};
}

vec2_t floor_vec2(vec2_t v) 
{
    return (vec2_t){floor(v.x), floor(v.y)};
}

vec2_t add_vec2(vec2_t v1, vec2_t v2) 
{
    return (vec2_t){v1.x + v2.x, v1.y + v2.y};
}

vec2_t subtract_vec2(vec2_t v1, vec2_t v2) 
{
    return (vec2_t){v1.x - v2.x, v1.y - v2.y};
}

vec2_t divide_vec2(vec2_t v, float f) 
{
    return (vec2_t){v.x / f, v.y / f};
}

vec2_t multiply_vec2(vec2_t v, float f) 
{
    return (vec2_t){v.x * f, v.y * f};
}

float length_vec2(vec2_t v) 
{
    return sqrt((v.x * v.x) + (v.y * v.y));
}

float dot_product(vec2_t v1, vec2_t v2) 
{
    return v1.x * v2.x + v1.y * v2.y;
}

SDL_Texture* load_texture(char* path, SDL_Renderer* renderer)
{
    SDL_Surface *optimizedSurface = NULL;
    SDL_Surface *surface = IMG_Load(path);
    if (surface == NULL) {
        printf("Unable to load image %s! SDL_image Error: %s", path, IMG_GetError());
    } else {
        optimizedSurface = SDL_ConvertSurfaceFormat(surface, SDL_PIXELFORMAT_RGBA8888, 0);
        if (optimizedSurface == NULL) {
            printf("Unable to optimize image %s! SDL Error: %s", path, SDL_GetError());
        }
        SDL_FreeSurface(surface);
    }
    SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, optimizedSurface);
    SDL_FreeSurface(optimizedSurface);
    return texture;
}
