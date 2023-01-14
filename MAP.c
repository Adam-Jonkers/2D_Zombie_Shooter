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

void generate_texture_map(int max_x, int max_y, float* randArray) {
    for (int y = 0; y < max_y; y++) {
        for (int x = 0; x < max_x; x++) {
            randArray[x + (max_x * y)] = (float)rand() / RAND_MAX;
        } 
    }
    printf("Generated texture map\n");
}

float fade(float t) {
    return t*t*t*(t*(t*6.0-15.0)+10.0);
}

vec2_t grad(vec2_t p, float* noiseMap, int width) {
    vec2_t v;
    v.x = noiseMap[(int)(p.x) + (width*(int)(p.y))] - noiseMap[(int)(p.x + 1) + (width * (int)(p.y))];
    v.y = noiseMap[(int)(p.x) + (width*(int)(p.y))] - noiseMap[(int)(p.x) + (width * (int)(p.y + 1))];
    return normalize_vec2(v);
}

float noise(vec2_t p, float* noiseMap, int width) {
    vec2_t p0 = floor_vec2(p);
    vec2_t p1 = add_vec2(p0, (vec2_t){1.0, 0.0});
    vec2_t p2 = add_vec2(p0, (vec2_t){0.0, 1.0});
    vec2_t p3 = add_vec2(p0, (vec2_t){1.0, 1.0});

    vec2_t g0 = grad(p0, noiseMap, width);
    vec2_t g1 = grad(p1, noiseMap, width);
    vec2_t g2 = grad(p2, noiseMap, width);
    vec2_t g3 = grad(p3, noiseMap, width);

    float t0 = p.x - p0.x;
    float fade_t0 = fade(t0);

    float t1 = p.y - p0.y;
    float fade_t1 = fade(t1);

    float p0p1 = (1.0 - fade_t0) * dot_product(g0, (subtract_vec2(p, p0))) + fade_t0 * dot_product(g1, (subtract_vec2(p, p1)));
    float p2p3 = (1.0 - fade_t0) * dot_product(g2, (subtract_vec2(p, p2))) + fade_t0 * dot_product(g3, (subtract_vec2(p, p3)));

    return ((1.0 - fade_t1) * p0p1 + fade_t1 * p2p3);
}

void Setup_Noise_Map(vec2_t max, float* noiseMap, float* randArray)
{
    // Setup noise map
    generate_texture_map(max.x, max.y, randArray);
    for (int y = 0; y < (int)max.y; y++) {
        for (int x = 0; x < (int)max.x; x++) {
            vec2_t i = (vec2_t){x, y};
            float n = noise(divide_vec2(i, FREQUENCY * 8), randArray, (int)max.x) * 1.0 +
            noise(divide_vec2(i, FREQUENCY * 4), randArray, (int)max.x) * 0.5 +
            noise(divide_vec2(i, FREQUENCY * 2), randArray, (int)max.x) * 0.25 +
            noise(divide_vec2(i, FREQUENCY), randArray, (int)max.x) * 0.125;
            noiseMap[x + ((int)max.x * y)] = n;
        }
    }
    free(randArray);
    printf("Generated noise map\n");
}

void Setup_Map_Png(bitmap_t map, float* noiseMap)
{
    // Setup map
    for (size_t y = 0; y < map.height; y++) {
        for (size_t x = 0; x < map.width; x++) {
            float n = noiseMap[x + (map.width * y)];
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
        printf("Failed to save map.png\n");
    } else {
        printf("Saved map.png\n");
    }
    
}

SDL_Texture* Load_Map_Texture(SDL_Renderer* renderer) 
{
    printf("Loaded map.png\n");
    return (load_texture("map.png", renderer));
}

void Draw_Map_Texture(SDL_Renderer* renderer, SDL_Texture* map_texture, Player_t* player, vec2_t windowSize)
{
    SDL_Rect src = {player->camera.x, player->camera.y, windowSize.x, windowSize.y};
    SDL_Rect dst = {0, 0, windowSize.x, windowSize.y};
    SDL_RenderCopy(renderer, map_texture, &src, &dst);
}

float Get_speed_multiplier(vec2_t position, vec2_t max, float* noiseMap)
{
    float speedMultiplier;
    float n = noiseMap[(int)position.x + ((int)max.x * (int)position.y)];
    if (n < -0.5) {
        speedMultiplier = 0.15;
    } else if (n < 0) {
        speedMultiplier = 1;
    } else if (n < 0.1) {
        speedMultiplier = 0.95;
    } else if (n < 0.2) {
        speedMultiplier = 0.9;
    } else if (n < 0.3) {
        speedMultiplier = 0.8;
    } else if (n < 0.4) {
        speedMultiplier = 0.7;
    } else if (n < 0.5) {
        speedMultiplier = 0.5;
    } else if (n >= 0.5) {
        speedMultiplier = 0.25;
    } else {
        speedMultiplier = 0;
    }
    return speedMultiplier;
}
