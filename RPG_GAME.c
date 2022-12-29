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
    // SETUP

    int Gamestate = 1;
    /*
    1 = Moving
    2 = Battle
    3 = Shutdown
    */

    bool playing = true;

    // Seed random number generator
    srand(time(NULL));

    // Setup display
    static const int max_y = MAP_HEIGHT;
    static const int max_x = MAP_WIDTH;
    int display_y;
    int display_x;
    static const int displaysize_y = 600;
    static const int displaysize_x = 600;

    // initilize SDL
    SDL_Init(SDL_INIT_VIDEO);

    // create a window
    SDL_Window *window = SDL_CreateWindow("Game", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, displaysize_x, displaysize_y, SDL_WINDOW_OPENGL);

    // create a renderer
    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);


    // Setup player
    Player player = Setup_Player();

    // Setup map
    player.player_x = get_random_number(displaysize_x / 2, max_x - displaysize_x / 2);
    player.player_y = get_random_number(displaysize_y / 2, max_y - displaysize_y / 2);

    display_x = player.player_x - displaysize_x / 2;
    display_y = player.player_y - displaysize_y / 2;
    float randarray[max_x][max_y];
    float noisemap[max_x][max_y];
    char map[max_x][max_y];
    Setup_Noise_Map(max_x, max_y, noisemap, randarray);
    Setup_Map(max_x, max_y, map, noisemap);

    // Display map
    Display_Map(max_x, max_y, map, &player, display_x, display_y, displaysize_x, displaysize_y);
    while (playing) {
        SDL_Event event;
        while (SDL_PollEvent(&event))
        {
            if (event.type == SDL_QUIT)
            {
                Gamestate = 3;
            }
        }
        if (Gamestate == 1)
        {
            // Move player
            Move_Player(max_x, max_y, map, &player, &Gamestate, &display_x, &display_y, displaysize_x, displaysize_y);
        } else if (Gamestate == 2)
        {
            bool result = battle(&player);
            if (result) {
                Gamestate = 1;
                Display_Map(max_x, max_y, map, &player, display_x, display_y, displaysize_x, displaysize_y);
            } else {
                Gamestate = 3;
            }
        } else if (Gamestate == 3)
        {
            SDL_DestroyRenderer(renderer);
            SDL_DestroyWindow(window);
            SDL_Quit();
            playing = false;
        }   
    }
}