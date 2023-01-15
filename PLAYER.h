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
    HitBox_t hitBox;
} Bullet_t;

typedef struct {
    Bullet_t** bullet;
    int num_bullets;
} Bullets_t;

typedef struct {
    int health;
    Animation_t* currentAnimation;
    Animation_t moveAnimation;
    Animation_t idleAnimation;
    SDL_FRect sprite;
    SDL_FPoint center;
    double rotation;
    float maxSpeed;
    float acceleration;
    vec2_t position;
    vec2_t velocity;
    vec2_t camera;
    Bullets_t bullets;
} Player_t;

typedef struct {
    int health;
    int damage;
    int attackRate;
    int attackRange;
    SDL_Texture* texture;
    SDL_FRect sprite;
    SDL_FPoint center;
    double rotation;
    float maxSpeed;
    vec2_t position;
    vec2_t velocity;
    float acceleration;
    int index;
    HitBox_t hitBox;
} Enemy_t;

typedef struct {
    Enemy_t** enemy;
    int num_enemys;
    int max_enemys;
} Enemys_t;

Player_t Setup_player(vec2_t windowSize, SDL_Renderer* renderer);

void Move_player(const Uint8* keyboard_state, Player_t* player, float dt, SDL_Renderer* renderer, Bullets_t* bullets, vec2_t windowSize, vec2_t max, mouse_t mouse, float* noiseMap);

void Draw_Player(SDL_Renderer* renderer, Player_t* player, float dt, vec2_t windowSize, vec2_t max);

void Create_Bullet(SDL_Renderer* renderer, Player_t* player, Bullets_t* bullets);

void Draw_Bullets(SDL_Renderer* renderer, Player_t* player);

void Draw_Bullet(SDL_Renderer* renderer, Bullet_t* bullet, Player_t* player);

void Destroy_Bullet(Bullet_t* bullet);

void Destroy_Bullets(Bullets_t* bullets);

void Remove_Bullet(Bullets_t* bullets, int index);

void Update_Bullets(Bullets_t* bullets, float dt, Enemys_t* enemys, Player_t* player, SDL_Renderer* renderer);

#endif