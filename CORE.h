#ifndef CORE_H
#define CORE_H

#include <stdint.h>

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

#define MAXANIMATION 20

typedef struct {
    float x;
    float y;
} vec2_t;

typedef struct {
    int length;
    SDL_Texture* animation[MAXANIMATION];
} Animation_t;

typedef struct {
    uint8_t red;
    uint8_t green;
    uint8_t blue; 
} pixel_t;

typedef struct {
    pixel_t *pixels;
    size_t width;
    size_t height;
} bitmap_t;

typedef struct {
    TTF_Font* font;
    SDL_Texture* texture;
    SDL_Rect textBox;
    char* text;
} Text_t;

int get_random_number(int min, int max);

vec2_t normalise_vec2(vec2_t v);

vec2_t floor_vec2(vec2_t v);

vec2_t add_vec2(vec2_t v1, vec2_t v2);

vec2_t subtract_vec2(vec2_t v1, vec2_t v2);

vec2_t divide_vec2(vec2_t v, float f);

vec2_t multiply_vec2(vec2_t v, float f);

float length_vec2(vec2_t v);

float dot_product(vec2_t v1, vec2_t v2);

SDL_Texture* load_texture(char* path, SDL_Renderer* renderer);

void load_animation(Animation_t *animation, char* path, SDL_Renderer* renderer);

int save_png_to_file (bitmap_t* bitmap, const char* path);

void load_Text(TTF_Font* font, char* text, SDL_Color textColor, SDL_Texture** textTexture, SDL_Renderer* renderer);

void load_Font(TTF_Font** font, char* path);

#endif