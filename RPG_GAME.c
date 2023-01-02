#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include "MAP.h"
#include "CORE.h"

int main(void)
{
    // initilize SDL
    SDL_Init(SDL_INIT_VIDEO);
    SDL_Window *window = SDL_CreateWindow("Game", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 600, 600, SDL_WINDOW_OPENGL | SDL_WINDOW_FULLSCREEN_DESKTOP);
    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    IMG_Init(IMG_INIT_PNG);

    // Load Images
    SDL_Surface *image = IMG_Load("image.png");
    SDL_Texture *image_texture = SDL_CreateTextureFromSurface(renderer, image);
    SDL_FreeSurface(image);

    SDL_DisplayMode dm;
    SDL_GetDesktopDisplayMode(0, &dm);

    SDL_FRect texture_destination;
    texture_destination.w = 400;
    texture_destination.h = 400;
    texture_destination.x = 100.0f;//(dm.w / 4);// - (texture_destination.w / 2);
    texture_destination.y = 100.0f;//(dm.h / 4);// - (texture_destination.h / 2);

    SDL_FPoint center;
    center.x = texture_destination.w / 2 + texture_destination.x - 100;
    center.y = texture_destination.h / 2 + texture_destination.y - 100;

    int max_x = dm.w;
    int max_y = dm.h;
    float rotation = 0.0f;

    srand(time(NULL));
    float* noisemap = calloc((dm.w * dm.h), sizeof(float));
    float* randarray = calloc((dm.w * dm.h), sizeof(float));
    char* map = calloc((dm.w * dm.h), sizeof(char));

    Setup_Noise_Map(max_x, max_y, noisemap, randarray);
    Setup_Map(max_x, max_y, map, noisemap);
    
    bool running = true;

    int thing = 0;

    while (running)
    {
        SDL_Event event;
        while (SDL_PollEvent(&event))
        {
            if (event.type == SDL_QUIT)
            {
                running = false;
            }
            if (event.type == SDL_KEYDOWN)
            {
                if (event.key.keysym.sym == SDLK_ESCAPE)
                {
                    running = false;
                }
            }
        }

        SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
        SDL_RenderClear(renderer);

        Draw_Map(max_x, max_y, map, renderer);

        SDL_RenderCopyExF(renderer, image_texture, NULL, &texture_destination, rotation, &center, SDL_FLIP_NONE);

        SDL_RenderPresent(renderer);
        rotation += 0.5f;
        printf("%d \n", thing);
        thing++;
    }
    
    SDL_DestroyTexture(image_texture);
    IMG_Quit();
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    free(noisemap);
    free(randarray);
    free(map);
    printf("\nQuit Successfully\n");
}