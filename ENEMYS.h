#ifndef ENEMYS_H
#define ENEMYS_H

#include "SDL2/SDL.h"
#include "SDL2/SDL_image.h"

#include "CORE.h"
#include "PLAYER.h"

typedef struct {
    int health;
    int damage;
    SDL_Texture* texture;
    SDL_FRect sprite;
    SDL_FPoint center;
    double rotation;
    float maxSpeed;
    vec2_t position;
    vec2_t velocity;
    float acceleration;
    int index;
} Enemy_t;

typedef struct {
    Enemy_t** enemy;
    int num_enemys;
} Enemys_t;

void Setup_enemy(Enemys_t* enemys, vec2_t windowSize, SDL_Renderer* renderer);

void Create_enemy(Enemys_t* enemys, vec2_t windowSize, SDL_Renderer* renderer);

void Draw_Enemy(SDL_Renderer* renderer, Enemy_t* enemy, vec2_t windowSize, Player_t* player);

void Draw_Enemys(SDL_Renderer* renderer, Enemys_t* enemys, vec2_t windowSize, Player_t* player);

void Destroy_Enemy(Enemy_t* enemy);

void Remove_Enemy(Enemys_t* enemys, int index);

void Remove_Enemys(Enemys_t* enemys);

void Update_Enemys(Enemys_t* enemys, Player_t* player, float dt);

#endif