#include <stdlib.h>
#include <math.h>
#include <ncurses.h>

#include "CORE.h"


int get_random_number(int min, int max)
{
    return (rand() % (max - min + 1) + min);
}

vec2 normalise(vec2 v) {
    float length = sqrt(v.x * v.x + v.y * v.y);
    return (vec2){v.x / length, v.y / length};
}

vec2 floor_vec2(vec2 v) {
    return (vec2){floor(v.x), floor(v.y)};
}

vec2 add_vec2(vec2 v1, vec2 v2) {
    return (vec2){v1.x + v2.x, v1.y + v2.y};
}

vec2 subtract_vec2(vec2 v1, vec2 v2) {
    return (vec2){v1.x - v2.x, v1.y - v2.y};
}

vec2 divide_vec2(vec2 v, float f) {
    return (vec2){v.x / f, v.y / f};
}

float dot_product(vec2 v1, vec2 v2) {
    return v1.x * v2.x + v1.y * v2.y;
}

void initilise_display(int* max_x, int* max_y) {
    initscr();
    getmaxyx(stdscr, *max_y, *max_x);
    max_x -= 10;
    max_y -= 5;
    start_color();
    init_color(8, 294, 714, 937);
    init_color(9, 204, 549, 192);
    init_color(10, 0, 200, 0);
    init_color(11, 100 * 4, 100 * 4, 100 * 4);
    init_color(12, 100 * 5, 100 * 5, 100 * 5);
    init_color(13, 100 * 6, 100 * 6, 100 * 6);
    init_color(14, 100 * 7, 100 * 7, 100 * 7);
    init_color(15, 1000, 1000, 1000);
    init_pair(1, 8, 8);
    init_pair(2, 9, 9);
    init_pair(3, 10, 10);
    init_pair(4, 11, 11);
    init_pair(5, 12, 12);
    init_pair(6, 13, 13);
    init_pair(7, 14, 14);
    init_pair(8, 15, 15);
}