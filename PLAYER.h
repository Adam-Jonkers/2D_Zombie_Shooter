#ifndef PLAYER_H
#define PLAYER_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdint.h>

#include "CORE.h"

typedef struct {
    SDL_Texture* texture;
    vec2_t position;
    vec2_t velocity;
    int index;
    uint32_t lifetime;
    uint32_t max_lifetime;
} Bullet_t;

typedef struct {
    Bullet_t** bullet;
    int num_bullets;
} Bullets_t;

typedef struct {
    int health;
    int damage;
    int ammo;
    int weapon;
    Animation_t* currentAnimation;
    Animation_t moveAnimation;
    Animation_t idleAnimation;
    SDL_FRect sprite;
    SDL_FPoint center;
    double rotation;
    float maxspeed;
    float acceleration;
    vec2_t position;
    vec2_t velocity;
    vec2_t camera;
    Bullets_t bullets;
} Player_t;

double mouse_angle(SDL_FRect sprite, mouse_t mouse);

Player_t Setup_player(vec2_t windowsize, SDL_Renderer* renderer);

float Get_speed_multiplyer(Player_t* player, vec2_t max, float* noisemap);

void Move_player(const Uint8* keyboard_state, Player_t* player, float timestep, SDL_Renderer* renderer, Bullets_t* bullets, vec2_t windowsize, vec2_t max, mouse_t mouse, float* noisemap);

void Draw_Player(SDL_Renderer* renderer, Player_t* player, float timestep, vec2_t windowsize, vec2_t max);

void Create_Bullet(SDL_Renderer* renderer, Player_t* player, Bullets_t* bullets);

void Draw_Bullets(SDL_Renderer* renderer, float timestep, Player_t* player);

void Draw_Bullet(SDL_Renderer* renderer, Bullet_t* bullet, float timestep, Player_t* player);

void Destroy_Bullet(Bullet_t* bullet);

void Destroy_Bullets(Bullets_t* bullets);

void Remove_Bullet(Bullets_t* bullets, int index);

void Update_Bullets(Bullets_t* bullets, float timestep);

#endif