#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <math.h>

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>

#include "RPG_GAME.h"
#include "MAP.h"
#include "CORE.h"
#include "PLAYER.h"
#include "ENEMYS.h"

#define MAP_WIDTH 5000
#define MAP_HEIGHT 5000

#define MAX_ENEMYS 500

// ms per spawn
#define SPAWN_RATE 5000 
#define DIFFICULTY_INCREASE_RATE 1000

Global_t Setup_Global()
{
    bool running = true;

    // initialize SDL
    SDL_Init(SDL_INIT_VIDEO);
    SDL_Window *window = SDL_CreateWindow("Game", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 600, 600, SDL_WINDOW_OPENGL | SDL_WINDOW_FULLSCREEN_DESKTOP);
    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    IMG_Init(IMG_INIT_PNG);
    TTF_Init();

    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);

    SDL_Surface *icon = IMG_Load("Assets/Icon/Icon.png");
    SDL_SetWindowIcon(window, icon);
    SDL_FreeSurface(icon);

    vec2_t windowSize;
    int window_width, window_height;

    SDL_GetWindowSize(window, &window_width, &window_height);
    windowSize.x = window_width;
    windowSize.y = window_height;

    printf("Display: y: %d, x: %d\n", window_height, window_width);

    mouse_t mouse;

    srand(time(NULL));

    TTF_Font* font = NULL;

    load_Font(&font, "Assets/Font/Font.ttf");

    Text_t fps = Setup_Text(font, (SDL_Color){255, 0, 0, 255}, NULL, (SDL_Rect){5, 30, 50, 30}, "FPS: ERROR");
    load_Text(&fps, renderer);

    Timer_t fpsTimer = create_timer();
    start_timer(&fpsTimer);

    Timer_t stepTimer = create_timer();
    start_timer(&stepTimer);
    float timeStep = 0.0f;

    const Uint8* keyboard_state = SDL_GetKeyboardState(NULL);
}

void Cleanup_Global(Global_t* global)
{
    TTF_CloseFont(font);
    IMG_Quit();
    TTF_Quit();
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}

Game_t Setup_Game()
{
    vec2_t max = {MAP_WIDTH, MAP_HEIGHT};

    SDL_Texture* loadingScreen = load_texture("Assets/Loading_Screen/LoadingScreen.png", renderer);
    SDL_RenderCopy(renderer, loadingScreen, NULL, NULL);
    SDL_RenderPresent(renderer);
    SDL_DestroyTexture(loadingScreen);

    Player_t player = Setup_player(windowSize, renderer);

    Enemys_t enemys;
    enemys.enemy = NULL;
    enemys.num_enemys = 0;
    enemys.max_enemys = MAX_ENEMYS;

    float* noiseMap = calloc((max.x * max.y), sizeof(float));
    float* randArray = calloc((max.x * max.y), sizeof(float));
    char* map = calloc((max.x * max.y), sizeof(char));
    bitmap_t bmap;
    bmap.height = max.y;
    bmap.width = max.x;
    bmap.pixels = calloc((max.x * max.y), sizeof(pixel_t));

    Text_t playerHealth = Setup_Text(font, (SDL_Color){255, 0, 0, 255}, NULL, (SDL_Rect){5, 0, 50, 30}, "HP: 100");
    load_Text(&playerHealth, renderer);

    Score_t score;

    score.score = 0;
    score.maxScore = 0;

    score.scoreText = Setup_Text(font, (SDL_Color){255, 0, 0, 255}, NULL, (SDL_Rect){windowSize.x - 50, 30, 50, 30}, "Score: 0");
    load_Text(&score.scoreText, renderer);
    score.scoreText.textBox.x = windowSize.x - score.scoreText.textBox.w;

    score.maxScoreText = Setup_Text(font, (SDL_Color){255, 0, 0, 255}, NULL, (SDL_Rect){windowSize.x - 80, 0, 80, 30}, "Highscore: ERROR");

    FILE* file = fopen("Highscore.txt", "r");
    if (file == NULL) {
        printf("No highscore file found\n");
        score.maxScore = 0;
        
    } else {
        fscanf(file, "%d", &score.maxScore);
        fclose(file);
    }

    sprintf(score.maxScoreText.text, "Highscore: %d", score.maxScore);
    load_Text(&score.maxScoreText, renderer);
    score.maxScoreText.textBox.x = windowSize.x - score.maxScoreText.textBox.w;

    Setup_Noise_Map(max, noiseMap, randArray);
    Setup_Map_Png(bmap, noiseMap);

    SDL_Texture* map_texture = Load_Map_Texture(renderer);

    Timer_t spawnTimer = create_timer();
    start_timer(&spawnTimer);

    Timer_t difficultyTimer = create_timer();
    start_timer(&difficultyTimer);
    u_int32_t difficultyIncreaseRate = DIFFICULTY_INCREASE_RATE;
    u_int32_t spawnRate = SPAWN_RATE;
}

void Run_Game(Game_t* game)
{
    if (spawnRate > 200 && get_time_ms(&difficultyTimer) > difficultyIncreaseRate) {
        spawnRate -= 60;
        start_timer(&difficultyTimer);
    }

    Spawn_Enemys(&enemys, windowSize, renderer, max, &spawnTimer, spawnRate, player.camera);

    Move_player(keyboard_state, &player, timeStep, renderer, &player.bullets, windowSize, max, mouse, noiseMap, &running);

    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
    SDL_RenderClear(renderer);

    Draw_Map_Texture(renderer, map_texture, &player, windowSize);

    Update_Bullets(&player.bullets, timeStep, &enemys, &player, renderer);

    Draw_Bullets(renderer, &player);

    Update_Enemys(&enemys, &player, timeStep, max, noiseMap, renderer, &score, windowSize, &playerHealth);

    Draw_Enemys(renderer, &enemys, windowSize, &player);

    Draw_Player(renderer, &player, timeStep, windowSize, max);

    Draw_Text(&playerHealth, renderer);

    Draw_Text(&fps, renderer);

    Draw_Text(&score.scoreText, renderer);

    Draw_Text(&score.maxScoreText, renderer);

    SDL_RenderPresent(renderer);
}

void close_Game(Game_t* game)
{
    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 75);
    SDL_RenderFillRect(renderer, NULL);

    SDL_Texture* GameOverScreen = load_texture("Assets/Game_Over_Screen/Game_Over.png", renderer);
    SDL_RenderCopy(renderer, GameOverScreen, NULL, NULL);
    SDL_RenderPresent(renderer);
    SDL_DestroyTexture(GameOverScreen);

    Remove_Enemys(&enemys);

    Remove_Bullets(&player.bullets);

    file = fopen("Highscore.txt", "w");
    fprintf(file,"%d",score.maxScore);
    fclose(file);

    bool wait = true;
    SDL_Event event1;
    while(wait) {
        while (SDL_PollEvent(&event1)){
            if(keyboard_state[SDL_SCANCODE_RETURN]) {
                wait = false;
            break;
        }
        }
    }

    free(noiseMap);
    free(map);

    if (remove("map.png")) {
        printf("Error deleting map file\n");
    }
}

int main(void)
{
    Setup_Global();

    Setup_Game();

    while (running) {
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                running = false;
            }
        }
        if (keyboard_state[SDL_SCANCODE_ESCAPE]) {
            running = false;
        }

        mouse.buttons = SDL_GetMouseState(&mouse.x, &mouse.y);

        get_fps(&fpsTimer, &fps, renderer);

        Run_Game();

        timeStep = get_time_ms(&stepTimer);
        start_timer(&stepTimer);
    }
    close_Game();

    Cleanup_Global();

    printf("Quit Successfully\n");
}