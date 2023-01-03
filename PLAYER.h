#ifndef PLAYER_H
#define PLAYER_H

#include "SDL2/SDL.h"
#include "SDL2/SDL_image.h"

#include "CORE.h"

typedef struct {
    SDL_Texture *texture;
    SDL_FRect sprite;
    SDL_FPoint center;
    double rotation;
    vec2 velocity;
    float maxspeed;
    float acceleration;
} Player;

double mouse_angle(SDL_FRect sprite);

#endif