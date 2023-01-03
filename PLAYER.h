#ifndef PLAYER_H
#define PLAYER_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include "CORE.h"

typedef struct {
    SDL_Texture *texture;
    SDL_FRect sprite;
    SDL_FPoint center;
    double rotation;
    float maxspeed;
    vec2_t position;
    vec2_t velocity;
    float acceleration;
} Player_t;

double mouse_angle(SDL_FRect sprite);

Player_t Setup_player(SDL_DisplayMode dm, SDL_Renderer* renderer);

void Move_player(const Uint8* keyboard_state, Player_t* player);

#endif