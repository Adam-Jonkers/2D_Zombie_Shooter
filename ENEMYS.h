#ifndef ENEMYS_H
#define ENEMYS_H

#include "SDL2/SDL.h"
#include "SDL2/SDL_image.h"

#include "CORE.h"

typedef struct {
    int health;
    int damage;
    Animation_t moveAnimation;
    Animation_t idleAnimation;
    SDL_FRect sprite;
    SDL_FPoint center;
    int action;
    double rotation;
    float maxspeed;
    vec2_t position;
    vec2_t velocity;
    float acceleration;
} Enemy_t;

#endif