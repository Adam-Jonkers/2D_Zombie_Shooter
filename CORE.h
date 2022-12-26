#ifndef CORE_H
#define CORE_H

typedef struct {
    float x;
    float y;
} vec2;

int get_random_number(int min, int max);

vec2 normalise(vec2 v);

vec2 floor_vec2(vec2 v);

vec2 add_vec2(vec2 v1, vec2 v2);

vec2 subtract_vec2(vec2 v1, vec2 v2);

vec2 divide_vec2(vec2 v, float f);

float dot_product(vec2 v1, vec2 v2);

void initilise_display(int* max_x, int* max_y);

#endif