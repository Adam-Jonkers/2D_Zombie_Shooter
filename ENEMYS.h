#ifndef ENEMYS_H
#define ENEMYS_H

#include "SDL2/SDL.h"
#include "SDL2/SDL_image.h"

#include "CORE.h"
#include "PLAYER.h"

void Setup_enemy(Enemys_t* enemys, vec2_t windowSize, SDL_Renderer* renderer, vec2_t max, vec2_t camera);

void Create_enemy(Enemys_t* enemys, vec2_t windowSize, SDL_Renderer* renderer, vec2_t max, vec2_t camera);

void Draw_Enemy(SDL_Renderer* renderer, Enemy_t* enemy, vec2_t windowSize, Player_t* player);

void Draw_Enemys(SDL_Renderer* renderer, Enemys_t* enemys, vec2_t windowSize, Player_t* player);

void Destroy_Enemy(Enemy_t* enemy);

void Remove_Enemy(Enemys_t* enemys, int index);

void Remove_Enemys(Enemys_t* enemys);

void Update_Enemy(Enemys_t* enemys, Player_t* player, float dt, vec2_t max, float* noiseMap, int index, SDL_Renderer* renderer, Score_t* score, vec2_t windowSize, Text_t* playerHealth);

void Update_Enemys(Enemys_t* enemys, Player_t* player, float dt, vec2_t max, float* noiseMap, SDL_Renderer* renderer, Score_t* score, vec2_t windowSize, Text_t* playerHealth);

void Spawn_Enemys(Enemys_t* enemys, vec2_t windowSize, SDL_Renderer* renderer, vec2_t max, Timer_t *timer, u_int32_t spawnRate, vec2_t camera);

#endif