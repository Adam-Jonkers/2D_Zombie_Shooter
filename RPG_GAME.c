#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include "MAP.h"
#include "CORE.h"

#define MAP_WIDTH 5000
#define MAP_HEIGHT 5000

int main(void)
{
    // initilize SDL
    SDL_Init(SDL_INIT_VIDEO);
    SDL_Window *window = SDL_CreateWindow("Game", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 600, 600, SDL_WINDOW_OPENGL | SDL_WINDOW_FULLSCREEN_DESKTOP);
    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    IMG_Init(IMG_INIT_PNG);

    // Load Images
    SDL_Surface *image = IMG_Load("Assets/Top_Down_Survivor/rifle/move/survivor-move_rifle_0.png");
    SDL_Texture *image_texture = SDL_CreateTextureFromSurface(renderer, image);
    SDL_FreeSurface(image);

    SDL_DisplayMode dm;
    SDL_GetDesktopDisplayMode(0, &dm);

    SDL_FRect texture_destination;
    texture_destination.w = 60;
    texture_destination.h = 60;
    texture_destination.x = dm.w / 2 - texture_destination.w / 2;
    texture_destination.y = dm.h / 2 - texture_destination.h / 2;

    SDL_FPoint center;
    center.x = 30;//dm.w / 2;//texture_destination.w / 2 + texture_destination.x - 100;
    center.y = 30;//dm.h / 2;//texture_destination.h / 2 + texture_destination.y - 100;

    int max_x = MAP_WIDTH;
    int max_y = MAP_HEIGHT;

    int player_x = 0;
    int player_y = 0;

    double rotation = 0.0;

    srand(time(NULL));

    float* noisemap = calloc((max_x * max_y), sizeof(float));
    float* randarray = calloc((max_x * max_y), sizeof(float));
    char* map = calloc((max_x * max_y), sizeof(char));

    Setup_Noise_Map(max_x, max_y, noisemap, randarray);
    Setup_Map(max_x, max_y, map, noisemap);
    
    bool running = true;

    const Uint8 *keyboard_state = SDL_GetKeyboardState(NULL);

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
        if (keyboard_state[SDL_SCANCODE_W] && player_y - 5 > 0)
        {
            player_y -= 5;
            //printf("W\n");
        }
        if (keyboard_state[SDL_SCANCODE_S] && player_y + 5 < MAP_HEIGHT)
        {
            player_y += 5;
            //printf("S\n");
        }
        if (keyboard_state[SDL_SCANCODE_A] && player_x - 5 > 0)
        {
            player_x -= 5;
            //printf("A\n");
        }
        if (keyboard_state[SDL_SCANCODE_D] && player_x + 5 < MAP_WIDTH)
        {
            player_x += 5;
            //printf("D\n");
        }

        rotation = mouse_angle(texture_destination);
        //printf("Rotation: %f\n", rotation);

        SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
        SDL_RenderClear(renderer);

        Draw_Map(max_x, max_y, map, renderer, player_x, player_y, dm.w, dm.h);

        SDL_RenderCopyExF(renderer, image_texture, NULL, &texture_destination, rotation, &center, SDL_FLIP_NONE);

        SDL_RenderPresent(renderer);

        //rotation += 1.0f;
        //printf("Texture X: %f, Texture Y: %f\n", texture_destination.x, texture_destination.y);
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