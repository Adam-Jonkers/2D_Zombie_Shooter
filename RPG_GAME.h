#ifndef RPG_GAME_H
#define RPG_GAME_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>

#include <stdbool.h>

#include "CORE.h"
#include "PLAYER.h"

typedef struct {
    bool running;
    SDL_Window* window;
    SDL_Renderer* renderer;
    SDL_Surface* icon;
    vec2_t windowSize;
    int window_width;
    int window_height;
    mouse_t mouse;
    TTF_Font* font;
    Text_t fpsText;
    Timer_t fpsTimer;
    Timer_t stepTimer;
    float dt;
    const Uint8* keyboard_state;
} Global_t;

typedef struct {
    vec2_t max;
    SDL_Texture* loadingScreen;
    Player_t player;
    Enemys_t enemys;
    float* noiseMap;
    float* randArray; 
    char* map; // is this actually needed?
    bitmap_t mapBitmap;
    Text_t playerHealthText;
    Score_t score;
    FILE* highscoreFile;
    SDL_Texture* mapTexture;
    Timer_t enemySpawnTimer;
    Timer_t difficultyTimer;
    u_int32_t difficultyIncreaseRate;
    u_int32_t spawnRate;
} Game_t;

Global_t Setup_Global(void);

void Cleanup_Global(Global_t* global);

Game_t Setup_Game(Global_t* global);

void Run_Game(Game_t* game, Global_t* global);

void close_Game(Game_t* game, Global_t* global);

#endif