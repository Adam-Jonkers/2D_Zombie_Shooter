#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <math.h>

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

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

    SDL_DisplayMode dm;
    SDL_GetDesktopDisplayMode(0, &dm);

    int max_x = MAP_WIDTH;
    int max_y = MAP_HEIGHT;

    Player_t player = Setup_player(dm, renderer);

    srand(time(NULL));

    float* noisemap = calloc((max_x * max_y), sizeof(float));
    float* randarray = calloc((max_x * max_y), sizeof(float));
    char* map = calloc((max_x * max_y), sizeof(char));

    Setup_Noise_Map(max_x, max_y, noisemap, randarray);
    Setup_Map(max_x, max_y, map, noisemap);
    
    bool running = true;

    const Uint8* keyboard_state = SDL_GetKeyboardState(NULL);

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

        Move_player(keyboard_state, &player);

        SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
        SDL_RenderClear(renderer);

        Draw_Map(max_x, max_y, map, renderer, player.position.x, player.position.y, dm.w, dm.h);

        SDL_RenderCopyExF(renderer, player.texture, NULL, &player.sprite, player.rotation * (180 / M_PI), &player.center, SDL_FLIP_NONE);

        SDL_RenderPresent(renderer);
    }
    
    SDL_DestroyTexture(player.texture);
    IMG_Quit();
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    free(noisemap);
    free(randarray);
    free(map);
    
    printf("\nQuit Successfully\n");
}