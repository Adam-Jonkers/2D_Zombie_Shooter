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
    vec2_t velocity;
    float maxspeed;
    float acceleration;
} Player_t;

double mouse_angle(SDL_FRect sprite);

Player_t Setup_player(SDL_DisplayMode dm, SDL_Renderer* renderer);

#endif