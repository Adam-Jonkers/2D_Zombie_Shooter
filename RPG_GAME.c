#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <SDL2/SDL.h>

#include "MAP.h"
#include "CHARACTERS.h"
#include "BATTLE.h"
#include "CORE.h"

int main(void)
{
    // initilize SDL
    SDL_Init(SDL_INIT_VIDEO);
    SDL_Window *window = SDL_CreateWindow("Game", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 600, 600, SDL_WINDOW_OPENGL | SDL_WINDOW_FULLSCREEN_DESKTOP);
    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

    SDL_DisplayMode dm;
    SDL_GetDesktopDisplayMode(0, &dm);

    int max_x = dm.w;
    int max_y = dm.h;

    srand(time(NULL));
    float* noisemap = calloc((dm.w * dm.h), sizeof(float));
    float* randarray = calloc((dm.w * dm.h), sizeof(float));
    char* map = calloc((dm.w * dm.h), sizeof(char));

    Setup_Noise_Map(max_x, max_y, noisemap, randarray);
    Setup_Map(max_x, max_y, map, noisemap);
    
    bool running = true;

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

        SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
        SDL_RenderClear(renderer);

        Draw_Map(max_x, max_y, map, renderer);

        SDL_RenderPresent(renderer);
    }
    
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    free(noisemap);
    free(randarray);
    free(map);
}