#ifndef PLAYER_H
#define PLAYER_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdint.h>
#include <stdbool.h>

#include "CORE.h"

typedef struct Player_t Player_t;

typedef struct {
    SDL_Texture* texture;
    vec2_t position;
    vec2_t velocity;
    int damage;
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
    SDL_Texture* icon;
    SDL_Rect rect;
    void (*upgrade)(Player_t* player, SDL_Renderer* renderer);
} Upgrade_t;

typedef struct {
    Upgrade_t** upgrades;
    int numberOfUpgrades;
} Upgrades_t;

typedef struct {
    float damage;
    float speedMultiplier;
    uint32_t attackRate;
    float attackRange;
} Weapon_t;

typedef struct Player_t{
    float health;
    Text_t playerHealthText;
    bool alive;
    enum {KNIFE, PISTOL, RIFLE, SHOTGUN} weapon;
    Weapon_t knife;
    Weapon_t pistol;
    Weapon_t rifle;
    Weapon_t shotgun;
    Weapon_t* currentWeapon;
    Timer_t attackTimer;
    bool attacking;
    Upgrade_t** availableUpgrades;
    Upgrades_t allUpgrades;
    Animation_t* currentAnimation;
    Animation_t moveAnimation;
    Animation_t idleAnimation;
    Animation_t attackAnimation;
    SDL_FRect sprite;
    SDL_FPoint center;
    double rotation;
    float maxSpeed;
    float acceleration;
    vec2_t position;
    vec2_t velocity;
    vec2_t camera;
    Bullets_t bullets;
    bool sprinting;
    float sprintMultiplier;
} Player_t;

typedef struct {
    float health;
    float damage;
    uint32_t attackRate;
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
    Timer_t attackTimer;
} Enemy_t;

typedef struct {
    Enemy_t** enemy;
    int num_enemys;
    int max_enemys;
} Enemys_t;

Player_t Setup_player(vec2_t windowSize, SDL_Renderer* renderer);

void Move_player(const Uint8* keyboard_state, Player_t* player, Enemys_t* enemys, float dt, SDL_Renderer* renderer, Bullets_t* bullets, vec2_t windowSize, vec2_t max, mouse_t mouse, float* noiseMap);

void Draw_Player(SDL_Renderer* renderer, Player_t* player, float dt, vec2_t windowSize, vec2_t max);

void Create_Bullet(SDL_Renderer* renderer, Bullets_t* bullets, vec2_t position, SDL_FRect sprite, float rotation, vec2_t velocity, SDL_FPoint center, float damage);

void Draw_Bullets(SDL_Renderer* renderer, Player_t* player);

void Draw_Bullet(SDL_Renderer* renderer, Bullet_t* bullet, Player_t* player);

void Destroy_Bullet(Bullet_t* bullet);

void Remove_Bullet(Bullets_t* bullets, int index);

void Remove_Bullets(Bullets_t* bullets);

void Update_Bullets(Bullets_t* bullets, float dt, Enemys_t* enemys, Player_t* player, SDL_Renderer* renderer);

void Upgrade_Health(Player_t* player, SDL_Renderer* renderer);

void Upgrade_Damage(Player_t* player, SDL_Renderer* renderer);

void Upgrade_Speed(Player_t* player, SDL_Renderer* renderer);

void Upgrade_Knife(Player_t* player, SDL_Renderer* renderer);

void Upgrade_Pistol(Player_t* player, SDL_Renderer* renderer);

void Upgrade_Rifle(Player_t* player, SDL_Renderer* renderer);

void Upgrade_Shotgun(Player_t* player, SDL_Renderer* renderer);

Upgrade_t* Setup_Upgrade_Health(SDL_Renderer* renderer);

Upgrade_t* Setup_Upgrade_Damage(SDL_Renderer* renderer);

Upgrade_t* Setup_Upgrade_Speed(SDL_Renderer* renderer);

Upgrade_t* Setup_Upgrade_Knife(SDL_Renderer* renderer);

Upgrade_t* Setup_Upgrade_Pistol(SDL_Renderer* renderer);

Upgrade_t* Setup_Upgrade_Rifle(SDL_Renderer* renderer);

Upgrade_t* Setup_Upgrade_Shotgun(SDL_Renderer* renderer);

void Setup_Upgrades(Upgrades_t* upgrades ,SDL_Renderer* renderer);

void SelectUpgrades(Upgrade_t* selectedUpgrades[3], Upgrades_t* upgrades, vec2_t windowSize, SDL_Renderer* renderer, int currentWeapon);

void freeUpgrades(Player_t* player);

void knifeAttack(Player_t* player, Enemys_t* enemys, SDL_Renderer* renderer);

void pistolAttack(Player_t* player, SDL_Renderer* renderer);

void rifleAttack(Player_t* player, SDL_Renderer* renderer);

void shotgunAttack(Player_t* player, SDL_Renderer* renderer);

#endif