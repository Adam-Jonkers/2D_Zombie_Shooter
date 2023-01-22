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

#define MAIN_MENU 0
#define SETUP_GAME 1
#define GAME 2
#define QUIT 3

Global_t Setup_Global(Game_t* game, MainMenu_t* mainMenu)
{
    Global_t global;

    global.gameState = MAIN_MENU;

    global.running = true;

    global.game = game;

    global.mainMenu = mainMenu;

    // initialize SDL
    SDL_Init(SDL_INIT_VIDEO);
    global.window = SDL_CreateWindow("Game", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 600, 600, SDL_WINDOW_OPENGL | SDL_WINDOW_FULLSCREEN_DESKTOP);
    global.renderer = SDL_CreateRenderer(global.window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    IMG_Init(IMG_INIT_PNG);
    TTF_Init();

    SDL_SetRenderDrawBlendMode(global.renderer, SDL_BLENDMODE_BLEND);

    global.icon = IMG_Load("Assets/Icon/Icon.png");
    SDL_SetWindowIcon(global.window, global.icon);
    SDL_FreeSurface(global.icon);

    SDL_GetWindowSize(global.window, &global.window_width, &global.window_height);
    global.windowSize.x = global.window_width;
    global.windowSize.y = global.window_height;

    printf("Display: y: %d, x: %d\n", global.window_height, global.window_width);

    srand(time(NULL));

    global.font = NULL;

    load_Font(&global.font, "Assets/Font/Font.ttf");

    global.fpsText = Setup_Text(global.font, (SDL_Color){255, 0, 0, 255}, NULL, (SDL_Rect){5, 30, 50, 30}, "FPS: ERROR");
    load_Text(&global.fpsText, global.renderer);

    global.fpsTimer = create_timer();
    start_timer(&global.fpsTimer);

    global.stepTimer = create_timer();
    start_timer(&global.stepTimer);
    global.dt = 0.0f;

    global.keyboard_state = SDL_GetKeyboardState(NULL);

    return global;
}

void Cleanup_Global(Global_t* global)
{
    TTF_CloseFont(global->font);
    IMG_Quit();
    TTF_Quit();
    SDL_DestroyRenderer(global->renderer);
    SDL_DestroyWindow(global->window);
    SDL_Quit();
}

void Setup_Main_Menu(MainMenu_t* mainMenu, Global_t* global)
{
    mainMenu->backgroundTexture = load_texture("Assets/Main_Menu/Background.png", global->renderer);
    mainMenu->playButtonText = Setup_Text(global->font, (SDL_Color){255, 0, 0, 255}, NULL, (SDL_Rect){5, 0, 50, 30}, "Play");
    load_Text(&mainMenu->playButtonText, global->renderer);

    mainMenu->quitButtonText = Setup_Text(global->font, (SDL_Color){255, 0, 0, 255}, NULL, (SDL_Rect){50, 0, 50, 30}, "Quit");
    load_Text(&mainMenu->quitButtonText, global->renderer);

    mainMenu->playButtonRect = (SDL_Rect){global->windowSize.x / 2 - 100, global->windowSize.y / 2 - 100, 200, 100};
    mainMenu->quitButtonRect = (SDL_Rect){global->windowSize.x / 2 - 100, global->windowSize.y / 2 + 100, 200, 100};
}

void Display_Main_Menu(MainMenu_t* mainMenu, Global_t* global)
{
    SDL_RenderClear(global->renderer);
    SDL_RenderCopy(global->renderer, mainMenu->backgroundTexture, NULL, NULL);

    SDL_SetRenderDrawColor(global->renderer, 0, 255, 0, 255);
    SDL_RenderFillRect(global->renderer, &mainMenu->playButtonRect);
    SDL_RenderCopy(global->renderer, mainMenu->playButtonText.texture, NULL, &mainMenu->playButtonRect);

    SDL_SetRenderDrawColor(global->renderer, 0, 0, 255, 255);
    SDL_RenderFillRect(global->renderer, &mainMenu->quitButtonRect);
    SDL_RenderCopy(global->renderer, mainMenu->quitButtonText.texture, NULL, &mainMenu->quitButtonRect);

    SDL_RenderPresent(global->renderer);

    if (global->mouse.buttons & SDL_BUTTON(SDL_BUTTON_LEFT) && Mouse_Over(&global->mouse, mainMenu->playButtonRect)) {
        global->gameState = SETUP_GAME;
    } else if (global->mouse.buttons & SDL_BUTTON(SDL_BUTTON_LEFT) && Mouse_Over(&global->mouse, mainMenu->quitButtonRect)) {
        global->gameState = QUIT;
    }
}

void Cleanup_Main_Menu(MainMenu_t* mainMenu)
{
    SDL_DestroyTexture(mainMenu->backgroundTexture);
    SDL_DestroyTexture(mainMenu->playButtonText.texture);
    SDL_DestroyTexture(mainMenu->quitButtonText.texture);
}

void Setup_Game(Game_t* game, Global_t* global)
{
    static bool generalComplete = false;
    static bool noiseMapComplete = false;
    static bool mapComplete = false;
    if (!generalComplete) {
        game->max = (vec2_t){MAP_WIDTH, MAP_HEIGHT};

        game->loadingScreen = load_texture("Assets/Loading_Screen/LoadingScreen.png", global->renderer);
        SDL_RenderCopy(global->renderer, game->loadingScreen, NULL, NULL);
        SDL_RenderPresent(global->renderer);
        SDL_DestroyTexture(game->loadingScreen);

        game->player = Setup_player(global->windowSize, global->renderer);

        game->enemys.enemy = NULL;
        game->enemys.num_enemys = 0;
        game->enemys.max_enemys = MAX_ENEMYS;

        game->noiseMap = calloc((game->max.x * game->max.y), sizeof(float));
        game->randArray = calloc((game->max.x * game->max.y), sizeof(float));
        game->map = calloc((game->max.x * game->max.y), sizeof(char));

        game->mapBitmap.height = game->max.y;
        game->mapBitmap.width = game->max.x;
        game->mapBitmap.pixels = calloc((game->max.x * game->max.y), sizeof(pixel_t));

        game->playerHealthText = Setup_Text(global->font, (SDL_Color){255, 0, 0, 255}, NULL, (SDL_Rect){5, 0, 50, 30}, "HP: 100");
        load_Text(&game->playerHealthText, global->renderer);

        game->score.score = 0;
        game->score.maxScore = 0;

        game->score.scoreText = Setup_Text(global->font, (SDL_Color){255, 0, 0, 255}, NULL, (SDL_Rect){global->windowSize.x - 50, 30, 50, 30}, "Score: 0");
        load_Text(&game->score.scoreText, global->renderer);
        game->score.scoreText.textBox.x = global->windowSize.x - game->score.scoreText.textBox.w;

        game->score.maxScoreText = Setup_Text(global->font, (SDL_Color){255, 0, 0, 255}, NULL, (SDL_Rect){global->windowSize.x - 80, 0, 80, 30}, "Highscore: ERROR");

        game->highscoreFile = fopen("Highscore.txt", "r");
        if (game->highscoreFile == NULL) {
            printf("No highscore file found\n");
            game->score.maxScore = 0;
            
        } else {
            fscanf(game->highscoreFile, "%d", &game->score.maxScore);
            fclose(game->highscoreFile);
        }

        sprintf(game->score.maxScoreText.text, "Highscore: %d", game->score.maxScore);
        load_Text(&game->score.maxScoreText, global->renderer);
        game->score.maxScoreText.textBox.x = global->windowSize.x - game->score.maxScoreText.textBox.w;

        game->enemySpawnTimer = create_timer();
        start_timer(&game->enemySpawnTimer);

        game->difficultyTimer = create_timer();
        start_timer(&game->difficultyTimer);
        game->difficultyIncreaseRate = DIFFICULTY_INCREASE_RATE;
        game->spawnRate = SPAWN_RATE;

        generalComplete = true;
    }

    if (!noiseMapComplete) {
        Setup_Noise_Map(game->max, game->noiseMap, game->randArray, &noiseMapComplete);
    } else if (!mapComplete) {
        Setup_Map_Png(game->mapBitmap, game->noiseMap, &mapComplete);
    } 

    if (mapComplete) {
        game->mapTexture = Load_Map_Texture(global->renderer);
        global->gameState = GAME;
        generalComplete = false;
        noiseMapComplete = false;
        mapComplete = false;
    }
}

void Run_Game(Game_t* game, Global_t* global)
{
    if (game->spawnRate > 200 && get_time_ms(&game->difficultyTimer) > game->difficultyIncreaseRate) {
        game->spawnRate -= 60;
        start_timer(&game->difficultyTimer);
    }

    Spawn_Enemys(&game->enemys, global->windowSize, global->renderer, game->max, &game->enemySpawnTimer, game->spawnRate, game->player.camera);

    Move_player(global->keyboard_state, &game->player, global->dt, global->renderer, &game->player.bullets, global->windowSize, game->max, global->mouse, game->noiseMap);

    SDL_SetRenderDrawColor(global->renderer, 255, 0, 0, 255);
    SDL_RenderClear(global->renderer);

    Draw_Map_Texture(global->renderer, game->mapTexture, &game->player, global->windowSize);

    Update_Bullets(&game->player.bullets, global->dt, &game->enemys, &game->player, global->renderer);

    Draw_Bullets(global->renderer, &game->player);

    Update_Enemys(&game->enemys, &game->player, global->dt, game->max, game->noiseMap, global->renderer, &game->score, global->windowSize, &game->playerHealthText);

    Draw_Enemys(global->renderer, &game->enemys, global->windowSize, &game->player);

    Draw_Player(global->renderer, &game->player, global->dt, global->windowSize, game->max);

    Draw_Text(&game->playerHealthText, global->renderer);

    Draw_Text(&global->fpsText, global->renderer);

    Draw_Text(&game->score.scoreText, global->renderer);

    Draw_Text(&game->score.maxScoreText, global->renderer);

    SDL_RenderPresent(global->renderer);

    if (!game->player.alive) {
        close_Game(game, global);
        global->gameState = MAIN_MENU;
    }
}

void close_Game(Game_t* game, Global_t* global)
{
    SDL_SetRenderDrawColor(global->renderer, 255, 0, 0, 75);
    SDL_RenderFillRect(global->renderer, NULL);

    game->GameOverScreen = load_texture("Assets/Game_Over_Screen/Game_Over.png", global->renderer);
    SDL_RenderCopy(global->renderer, game->GameOverScreen, NULL, NULL);
    SDL_RenderPresent(global->renderer);
    SDL_DestroyTexture(game->GameOverScreen);

    Remove_Enemys(&game->enemys);

    Remove_Bullets(&game->player.bullets);

    game->highscoreFile = fopen("Highscore.txt", "w");
    fprintf(game->highscoreFile,"%d",game->score.maxScore);
    fclose(game->highscoreFile);

    bool wait = true;
    SDL_Event event1;
    while(wait) {
        while (SDL_PollEvent(&event1)){
            if(global->keyboard_state[SDL_SCANCODE_RETURN]) {
                wait = false;
            break;
        }
        }
    }

    free(game->noiseMap);
    free(game->map);

    if (remove("map.png")) {
        printf("Error deleting map file\n");
    }
}

int main(void)
{
    Game_t game;

    MainMenu_t mainMenu;

    Global_t global = Setup_Global(&game, &mainMenu);

    Setup_Main_Menu(&mainMenu, &global);

    while (global.running) {
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                global.gameState = QUIT;
            }
        }
        if (global.keyboard_state[SDL_SCANCODE_ESCAPE]) {
            global.gameState = QUIT;
        }


        switch (global.gameState) {
        case MAIN_MENU:
            Display_Main_Menu(&mainMenu, &global);
            break;

        case SETUP_GAME:
            Setup_Game(&game, &global);
            break;
        
        case GAME:
            Run_Game(&game, &global);
            break;

        case QUIT:
            Cleanup_Global(&global);
            global.running = false;
            printf("Quit Successfully\n");
            break;

        default:
            printf("Error: No game state\n");
            break;
        }

        global.mouse.buttons = SDL_GetMouseState(&global.mouse.x, &global.mouse.y);

        get_fps(&global.fpsTimer, &global.fpsText, global.renderer);

        global.dt = get_time_ms(&global.stepTimer);
        start_timer(&global.stepTimer);
    }
}