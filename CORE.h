#ifndef CORE_H
#define CORE_H

#include <stdint.h>
#include <stdbool.h>

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
    int speed;
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
    SDL_Color textColor;
    SDL_Texture* texture;
    SDL_Rect textBox;
    char text[100];
} Text_t;

typedef struct {
    uint32_t startTicks;
    uint32_t pausedTicks;
    bool paused;
    bool started;
} Timer_t;

typedef struct {
    int x;
    int y;
    Uint32 buttons;
} mouse_t;

typedef struct {
    int w;
    int h;
} HitBox_t;

int get_random_number(int min, int max);

vec2_t normalize_vec2(vec2_t v);

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

void load_Text(Text_t* text, SDL_Renderer* renderer);

void load_Font(TTF_Font** font, char* path);

Timer_t create_timer(void);

void start_timer(Timer_t* timer);

void stop_timer(Timer_t* timer);

void pause_timer(Timer_t* timer);

void unpause_timer(Timer_t* timer);

u_int32_t get_time_ms(Timer_t* timer);

float get_fps(Timer_t* timer, Text_t* fps_text, SDL_Renderer* renderer);

double get_angle(vec2_t p1, vec2_t p2);

bool check_collision(vec2_t position1, HitBox_t hitBox1, vec2_t position2, HitBox_t hitBox2);

#endif