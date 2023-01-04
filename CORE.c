#include <stdlib.h>
#include <math.h>
#include <string.h>
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
    SDL_Surface *surface = IMG_Load(path);
    if (surface == NULL) {
        printf("Unable to load image %s! SDL_image Error: %s\n", path, IMG_GetError());
    }
    SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);
    return texture;
}

void load_animation(Animation_t *animation, char* path, SDL_Renderer* renderer)
{
    for (int i = 0; i <= animation->length; i++)
    {
        char fullPath[100];
        strcpy(fullPath, path);
        char index[20];
        sprintf(index, "%d", i);
        strcat(fullPath, index);
        strcat(fullPath, ".png");
        animation->animation[i] = load_texture(fullPath, renderer);
    }
}