#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <unistd.h>

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include "MAP.h"
#include "CORE.h"
#include "PLAYER.h"

#define FREQUENCY 75

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

vec2_t grad(vec2_t p, float* noisemap, int width) {
    vec2_t v;
    v.x = noisemap[(int)(p.x) + (width*(int)(p.y))] - noisemap[(int)(p.x + 1) + (width * (int)(p.y))];
    v.y = noisemap[(int)(p.x) + (width*(int)(p.y))] - noisemap[(int)(p.x) + (width * (int)(p.y + 1))];
    return normalise_vec2(v);
}

float noise(vec2_t p, float* noisemap, int width) {
    vec2_t p0 = floor_vec2(p);
    vec2_t p1 = add_vec2(p0, (vec2_t){1.0, 0.0});
    vec2_t p2 = add_vec2(p0, (vec2_t){0.0, 1.0});
    vec2_t p3 = add_vec2(p0, (vec2_t){1.0, 1.0});

    vec2_t g0 = grad(p0, noisemap, width);
    vec2_t g1 = grad(p1, noisemap, width);
    vec2_t g2 = grad(p2, noisemap, width);
    vec2_t g3 = grad(p3, noisemap, width);

    float t0 = p.x - p0.x;
    float fade_t0 = fade(t0);

    float t1 = p.y - p0.y;
    float fade_t1 = fade(t1);

    float p0p1 = (1.0 - fade_t0) * dot_product(g0, (subtract_vec2(p, p0))) + fade_t0 * dot_product(g1, (subtract_vec2(p, p1)));
    float p2p3 = (1.0 - fade_t0) * dot_product(g2, (subtract_vec2(p, p2))) + fade_t0 * dot_product(g3, (subtract_vec2(p, p3)));

    return ((1.0 - fade_t1) * p0p1 + fade_t1 * p2p3);
}

void Setup_Noise_Map(vec2_t max, float* noisemap, float* randarray)
{
    // Setup noise map
    generate_texture_map(max.x, max.y, randarray);
    printf("Made Texture Map\n");
    for (int y = 0; y < (int)max.y; y++)
    {
        for (int x = 0; x < (int)max.x; x++)
        {
            vec2_t i = (vec2_t){x, y};
            float n = noise(divide_vec2(i, FREQUENCY * 8), randarray, (int)max.x) * 1.0 +
            noise(divide_vec2(i, FREQUENCY * 4), randarray, (int)max.x) * 0.5 +
            noise(divide_vec2(i, FREQUENCY * 2), randarray, (int)max.x) * 0.25 +
            noise(divide_vec2(i, FREQUENCY), randarray, (int)max.x) * 0.125;
            noisemap[x + ((int)max.x * y)] = n;
        }
    }
}

void Setup_Map_Png(bitmap_t map, float* noisemap)
{
    // Setup map
    for (size_t y = 0; y < map.height; y++)
    {
        for (size_t x = 0; x < map.width; x++)
        {
            float n = noisemap[x + (map.width * y)];
            if (n < -0.5) {
                map.pixels[x + (map.width * y)] = (pixel_t){75, 182, 239};
            } else if (n < 0) {
                map.pixels[x + (map.width * y)] = (pixel_t){52, 140, 49};
            } else if (n < 0.1) {
                map.pixels[x + (map.width * y)] = (pixel_t){0, 51, 0};
            } else if (n < 0.2) {
                map.pixels[x + (map.width * y)] = (pixel_t){102, 102, 102};
            } else if (n < 0.3) {
                map.pixels[x + (map.width * y)] = (pixel_t){127, 127, 127};
            } else if (n < 0.4) {
                map.pixels[x + (map.width * y)] = (pixel_t){153, 153, 153};
            } else if (n < 0.5) {
                map.pixels[x + (map.width * y)] = (pixel_t){178, 178, 178};
            } else if (n >= 0.5) {
                map.pixels[x + (map.width * y)] = (pixel_t){255, 255, 255};
            } else {
                map.pixels[x + (map.width * y)] = (pixel_t){255, 0, 0};
                printf("Error: %f\n", n);
            }
        }
    }
    if (save_png_to_file(&map, "map.png") != 0) {
        printf("Failed to save map.png");
    }
}

SDL_Texture* Load_Map_Texture(SDL_Renderer* renderer) 
{
    return (load_texture("map.png", renderer));
}

void Draw_Map_Texture(SDL_Renderer* renderer, SDL_Texture* map_texture, Player_t* player, vec2_t windowsize)
{
    SDL_Rect src = {player->camera.x, player->camera.y, windowsize.x, windowsize.y};
    SDL_Rect dst = {0, 0, windowsize.x, windowsize.y};
    SDL_RenderCopy(renderer, map_texture, &src, &dst);
}
