#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <unistd.h>
#include <SDL2/SDL.h>

#include "MAP.h"
#include "CHARACTERS.h"
#include "CORE.h"

#define FREQUENCY 8

void generate_texture_map(int max_x, int max_y, float* randarray) {
    for (int y = 0; y < max_y; y++) {
        for (int x = 0; x < max_x; x++)
        {
            randarray[x + (max_x * y)] = (float)rand() / RAND_MAX;
        } 
    }
}

float fade(float t) {
    return t*t*t*(t*(t*6.0-15.0)+10.0);
}

vec2 grad(vec2 p, float* noisemap, int width) {
    vec2 v;
    v.x = noisemap[(int)(p.x) + (width*(int)(p.y))] - noisemap[(int)(p.x + 1) + (width * (int)(p.y))];
    v.y = noisemap[(int)(p.x) + (width*(int)(p.y))] - noisemap[(int)(p.x) + (width * (int)(p.y + 1))];
    return normalise(v);
}

float noise(vec2 p, float* noisemap, int width) {
    vec2 p0 = floor_vec2(p);
    vec2 p1 = add_vec2(p0, (vec2){1.0, 0.0});
    vec2 p2 = add_vec2(p0, (vec2){0.0, 1.0});
    vec2 p3 = add_vec2(p0, (vec2){1.0, 1.0});

    vec2 g0 = grad(p0, noisemap, width);
    vec2 g1 = grad(p1, noisemap, width);
    vec2 g2 = grad(p2, noisemap, width);
    vec2 g3 = grad(p3, noisemap, width);

    float t0 = p.x - p0.x;
    float fade_t0 = fade(t0);

    float t1 = p.y - p0.y;
    float fade_t1 = fade(t1);

    float p0p1 = (1.0 - fade_t0) * dot_product(g0, (subtract_vec2(p, p0))) + fade_t0 * dot_product(g1, (subtract_vec2(p, p1)));
    float p2p3 = (1.0 - fade_t0) * dot_product(g2, (subtract_vec2(p, p2))) + fade_t0 * dot_product(g3, (subtract_vec2(p, p3)));

    return ((1.0 - fade_t1) * p0p1 + fade_t1 * p2p3);
}

void Setup_Noise_Map(int max_x, int max_y,float* noisemap, float* randarray)
{
    // Setup noise map
    printf(" max_x: %d, max_y: %d", max_x, max_y);
    sleep(1);
    generate_texture_map(max_x, max_y, randarray);
    printf("Made Texture Map\n");
    for (int y = 0; y < max_y; y++)
    {
        for (int x = 0; x < max_x; x++)
        {
            printf("x: %d, y: %d\n", x, y);
            vec2 i = (vec2){x, y};
            float n = noise(divide_vec2(i, FREQUENCY * 8), randarray, max_x) * 1.0 +
            noise(divide_vec2(i, FREQUENCY * 4), randarray, max_x) * 0.5 +
            noise(divide_vec2(i, FREQUENCY * 2), randarray, max_x) * 0.25 +
            noise(divide_vec2(i, FREQUENCY), randarray, max_x) * 0.125;
            noisemap[x + (max_x * y)] = n;
        }
    }
    printf(" max_x: %d, max_y: %d\n", max_x, max_y);
}

void Setup_Map(int max_x, int max_y ,char* map, float* noisemap)
{
    // Setup map
    for (int y = 0; y < max_y; y++)
    {
        for (int x = 0; x < max_x; x++)
        {
            float n = noisemap[x + (max_x * y)];
            if (n < -0.5) {
                map[x + (max_x * y)] = '~';
            } else if (n < 0) {
                map[x + (max_x * y)] = '-';
            } else if (n < 0.1) {
                map[x + (max_x * y)] = ' ';
            } else if (n < 0.2) {
                map[x + (max_x * y)] = 'L';
            } else if (n < 0.3) {
                map[x + (max_x * y)] = 'M';
            } else if (n < 0.4) {
                map[x + (max_x * y)] = 'H';
            } else if (n < 0.5) {
                map[x + (max_x * y)] = '^';
            } else if (n < 0.8) {
                map[x + (max_x * y)] = '+';
            } else {
                map[x + (max_x * y)] = '+';
            }
        }
    }
}

void Draw_Map(int max_x, int max_y, char* map, SDL_Renderer* renderer)
{
    char c;
    printf("Drawing Map\n");
    //int screenx;
    //int screeny;
    // Display map
    for (int y = 0/*display_y*/; y < /*displaysize_y + display_y && y <=*/ max_y; y++)
    {
        for (int x = 0/*display_x*/; x < /*displaysize_x + display_x && x <=*/ max_x; x++)
        {
            //screenx = x - display_x;
            //screeny = y - display_y;
            c = map[x + (max_x * y)];
            if (c == '~') {
                SDL_SetRenderDrawColor(renderer, 75, 182, 239, 255);
                SDL_RenderDrawPoint(renderer, x, y);
            } else if (c == '-') {
                SDL_SetRenderDrawColor(renderer, 52, 140, 49, 255);
                SDL_RenderDrawPoint(renderer, x, y);
            } else if (c == ' ') {
                SDL_SetRenderDrawColor(renderer, 0, 51, 0, 255);
                SDL_RenderDrawPoint(renderer, x, y);
            } else if (c == 'L') {
                SDL_SetRenderDrawColor(renderer, 102, 102, 102, 255);
                SDL_RenderDrawPoint(renderer, x, y);
            } else if (c == 'M') {
                SDL_SetRenderDrawColor(renderer, 127, 127, 127, 255);
                SDL_RenderDrawPoint(renderer, x, y);
            } else if (c == 'H') {
                SDL_SetRenderDrawColor(renderer, 153, 153, 153, 255);
                SDL_RenderDrawPoint(renderer, x, y);
            } else if (c == '^') {
                SDL_SetRenderDrawColor(renderer, 178, 178, 178, 255);
                SDL_RenderDrawPoint(renderer, x, y);
            } else if (c == '+') {
                SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
                SDL_RenderDrawPoint(renderer, x, y);
            } else {
                SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
                SDL_RenderDrawPoint(renderer, x, y);
            }
        }
    }
}


// void Move_Player(int max_x, int max_y, char map[max_x][max_y], Player* player, int* Gamestate, int* display_x, int* display_y, int displaysize_x, int displaysize_y)
// {
//     // Move player
//     flushinp();
//     bool valid_move = false;
//     while (valid_move == false)
//     {
//     int action = getch();
//     if (action == KEY_DOWN && player->player_y + 1 < max_y) {
//         player->player_y += 1;
//         valid_move = true;
//     } else if (action == KEY_UP && player->player_y - 1 >= 0) {
//         player->player_y -= 1;
//         valid_move = true;
//     } else if (action == KEY_LEFT && player->player_x - 1 >= 0) {
//         player->player_x -= 1;
//         valid_move = true;
//     } else if (action == KEY_RIGHT && player->player_x + 2 < max_x) {
//         player->player_x += 1;
//         valid_move = true;
//     }
//     }
//     *display_y = player->player_y - displaysize_y / 2;
//     *display_x = player->player_x - displaysize_x / 2;
    
//     if (*display_x < 0) {
//         *display_x = 0;
//     } else if (*display_x > max_x - displaysize_x) {
//         *display_x = max_x - displaysize_x;
//     }

//     if (*display_y < 0) {
//         *display_y = 0;
//     } else if (*display_y > max_y - displaysize_y) {
//         *display_y = max_y - displaysize_y;
//     }
    

//     int battlechance = 0;
//     switch (map[player->player_x][player->player_y])
//     {
//     case '~':
//         battlechance = 0;
//         break;
//     case '-':
//         battlechance = 0;
//         break;
//     case ' ':
//         battlechance = 0;
//         break;
//     case 'L':
//         battlechance = 0;
//         break;
//     case 'M':
//         battlechance = 0;
//         break;
//     case 'H':
//         battlechance = 0;
//         break;
//     case '^':
//         battlechance = 0;
//         break;
//     case '+':
//         battlechance = 0;
//         break;
//     }
//     int battleRNG = get_random_number(0, 100);   
//     if (battleRNG < battlechance)
//     {
//         *Gamestate = 2;
//     }
//     usleep(50000);
//     Display_Map(max_x, max_y, map, player, *display_x, *display_y, displaysize_x, displaysize_y);

    
// }