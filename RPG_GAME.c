#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <math.h>

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>

#include "MAP.h"
#include "CORE.h"
#include "PLAYER.h"

#define MAP_WIDTH 5000
#define MAP_HEIGHT 5000

int main(void)
{
    // initilize SDL
    SDL_Init(SDL_INIT_VIDEO);
    SDL_Window *window = SDL_CreateWindow("Game", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 600, 600, SDL_WINDOW_OPENGL | SDL_WINDOW_FULLSCREEN_DESKTOP);
    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    IMG_Init(IMG_INIT_PNG);
    TTF_Init();

    SDL_Surface *icon = IMG_Load("Assets/Icon/Icon.png");
    SDL_SetWindowIcon(window, icon);
    SDL_FreeSurface(icon);

    int window_x, window_y;

    SDL_GetWindowSize(window, &window_x, &window_y);

    printf("Display: y: %d, x: %d\n", window_y, window_x);

    int max_x = MAP_WIDTH;
    int max_y = MAP_HEIGHT;

    SDL_Texture* loadingScreen = load_texture("Assets/Loading_Screen/LoadingScreen.png", renderer);
    SDL_RenderCopy(renderer, loadingScreen, NULL, NULL);
    SDL_RenderPresent(renderer);
    SDL_DestroyTexture(loadingScreen);

    Player_t player = Setup_player(window_x, window_y, renderer);

    srand(time(NULL));

    float* noisemap = calloc((max_x * max_y), sizeof(float));
    float* randarray = calloc((max_x * max_y), sizeof(float));
    char* map = calloc((max_x * max_y), sizeof(char));
    bitmap_t bmap;
    bmap.height = max_y;
    bmap.width = max_x;
    bmap.pixels = calloc((max_x * max_y), sizeof(pixel_t));

    TTF_Font* font = NULL;

    load_Font(&font, "Assets/Font/Font.ttf");

    Text_t fps;
    fps.font = font;
    fps.textColor = (SDL_Color){255, 0, 0, 255};
    fps.texture = NULL;
    fps.textBox.x = 5;
    fps.textBox.y = 0;
    fps.textBox.w = 50;
    fps.textBox.h = 30;
    strcpy(fps.text, "FPS: ERROR");

    load_Text(&fps, renderer);

    Setup_Noise_Map(max_x, max_y, noisemap, randarray);
    Setup_Map_Png(bmap, noisemap);
    
    bool running = true;

    Timer_t fpsTimer = create_timer();
    start_timer(&fpsTimer);

    Timer_t stepTimer = create_timer();
    start_timer(&stepTimer);
    float timeStep = 0.0f;

    const Uint8* keyboard_state = SDL_GetKeyboardState(NULL);

    SDL_Texture* map_texture = Load_Map_Texture(renderer);

    while (running)
    {
        SDL_Event event;
        while (SDL_PollEvent(&event))
        {
            if (event.type == SDL_QUIT)
            {
                running = false;
            }
        }
        if (keyboard_state[SDL_SCANCODE_ESCAPE])
        {
            running = false;
        }

        get_fps(&fpsTimer, &fps, renderer);

        Move_player(keyboard_state, &player, timeStep);

        SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
        SDL_RenderClear(renderer);

        Draw_Map_Texture(renderer, map_texture, player.position.x, player.position.y, window_x, window_y);

        Draw_Player(renderer, &player);

        SDL_RenderCopy(renderer, fps.texture, NULL, &fps.textBox);

        SDL_RenderPresent(renderer);

        timeStep = get_time_ms(&stepTimer);
        start_timer(&stepTimer);
    }
    TTF_CloseFont(font);
    IMG_Quit();
    TTF_Quit();
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    free(noisemap);
    free(randarray);
    free(map);

    if (remove("map.png")) {
        printf("Error deleting map file");
    }
    
    printf("\nQuit Successfully\n");
}