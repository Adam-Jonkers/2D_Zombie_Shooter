#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <ncursesw/ncurses.h>
#include <unistd.h>

#include "MAP.h"
#include "CHARACTERS.h"
#include "CORE.h"

void generate_texture_map(int max_x, int max_y, float randarray[max_x][max_y]) {
    for (int x = 0; x < max_x; x++) {
        for (int y = 0; y < max_y; y++)
        {
            randarray[x][y] = (float)rand() / RAND_MAX;
        } 
    }
}

float fade(float t) {
    return t*t*t*(t*(t*6.0-15.0)+10.0);
}

vec2 grad(vec2 p, int max_x, int max_y, float noisemap[max_x][max_y]) {
    vec2 v;
    v.x = noisemap[(int)p.x][(int)p.y] - noisemap[((int)p.x) + 1][(int)p.y];
    v.y = noisemap[(int)p.x][(int)p.y] - noisemap[(int)p.x][((int)p.y) + 1];
    return normalise(v);
}

float noise(vec2 p, int max_x, int max_y, float noisemap[max_x][max_y]) {
    vec2 p0 = floor_vec2(p);
    vec2 p1 = add_vec2(p0, (vec2){1.0, 0.0});
    vec2 p2 = add_vec2(p0, (vec2){0.0, 1.0});
    vec2 p3 = add_vec2(p0, (vec2){1.0, 1.0});

    vec2 g0 = grad(p0, max_x, max_y, noisemap);
    vec2 g1 = grad(p1, max_x, max_y, noisemap);
    vec2 g2 = grad(p2, max_x, max_y, noisemap);
    vec2 g3 = grad(p3, max_x, max_y, noisemap);

    float t0 = p.x - p0.x;
    float fade_t0 = fade(t0);

    float t1 = p.y - p0.y;
    float fade_t1 = fade(t1);

    float p0p1 = (1.0 - fade_t0) * dot_product(g0, (subtract_vec2(p, p0))) + fade_t0 * dot_product(g1, (subtract_vec2(p, p1)));
    float p2p3 = (1.0 - fade_t0) * dot_product(g2, (subtract_vec2(p, p2))) + fade_t0 * dot_product(g3, (subtract_vec2(p, p3)));

    return (1.0 - fade_t1) * p0p1 + fade_t1 * p2p3;
}

void Setup_Noise_Map(int max_x, int max_y,float noisemap[max_x][max_y], float randarray[max_x][max_y])
{
    wclear(stdscr);
    printw("Generating noise map...\n\n");
    wrefresh(stdscr);
    // Setup noise map
    generate_texture_map(max_x, max_y, randarray);
    for (int y = 0; y < max_y; y++)
    {
        for (int x = 0; x < max_x; x++)
        {
            vec2 i = (vec2){x, y};
            float n = noise(divide_vec2(i, 64), max_x, max_y, randarray) * 1.0 +
            noise(divide_vec2(i, 32), max_x, max_y, randarray) * 0.5 +
            noise(divide_vec2(i, 16), max_x, max_y, randarray) * 0.25 +
            noise(divide_vec2(i, 8), max_x, max_y, randarray) * 0.125;
            noisemap[x][y] = n;
        }
    }
}

void Setup_Map(int max_x, int max_y ,char map[max_x][max_y], float noisemap[max_x][max_y])
{
    wclear(stdscr);
    printw("Generating map...\n\n");
    wrefresh(stdscr);
    // Setup map
    for (int y = 0; y < max_y; y++)
    {
        for (int x = 0; x < max_x; x++)
        {
            float n = noisemap[x][y];
            if (n < -0.5) {
                map[x][y] = '~';
            } else if (n < 0) {
                map[x][y] = '-';
            } else if (n < 0.1) {
                map[x][y] = ' ';
            } else if (n < 0.2) {
                map[x][y] = 'L';
            } else if (n < 0.3) {
                map[x][y] = 'M';
            } else if (n < 0.4) {
                map[x][y] = 'H';
            } else if (n < 0.5) {
                map[x][y] = '^';
            } else if (n < 0.8) {
                map[x][y] = '+';
            } else {
                map[x][y] = '+';
            }
        }
    }
}

void Display_Map(int max_x, int max_y, char map[max_x][max_y], Player* player, int display_x, int display_y, int displaysize_x, int displaysize_y)
{
    char c;
    int screenx;
    int screeny;
    const wchar_t* star = L"\U0001F469";
    wclear(stdscr);
    // Display map
    for (int y = display_y; y < displaysize_y + display_y && y <= max_y; y++)
    {
        for (int x = display_x; x < displaysize_x + display_x && x <= max_x; x++)
        {
            screenx = x - display_x;
            screeny = y - display_y;
            if (x == player->player_x +1 && y == player->player_y) {
                
            } else {
                c = map[x][y];
                if (c == '~') {
                    attron(COLOR_PAIR(LAKE_PAIR));
                    if (x == player->player_x && y == player->player_y) {   
                        mvaddwstr(screeny, screenx, star);
                    } else {
                        mvaddch(screeny, screenx, '~'); 
                    }
                    attroff(COLOR_PAIR(LAKE_PAIR));
                } else if (c == '-') {
                    attron(COLOR_PAIR(PLAINS_PAIR));
                    if (x == player->player_x && y == player->player_y) {   
                        mvaddwstr(screeny, screenx, star);
                    } else {
                        mvaddch(screeny, screenx, '-');
                    }
                    attroff(COLOR_PAIR(PLAINS_PAIR));
                } else if (c == ' ') {
                    attron(COLOR_PAIR(FOREST_PAIR));
                    if (x == player->player_x && y == player->player_y) {   
                        mvaddwstr(screeny, screenx, star);
                    } else {
                        mvaddch(screeny, screenx, ' '); 
                    }                    
                    attroff(COLOR_PAIR(FOREST_PAIR));
                } else if (c == 'L') {
                    attron(COLOR_PAIR(MOUNTAIN_1_PAIR));
                    if (x == player->player_x && y == player->player_y) {   
                        mvaddwstr(screeny, screenx, star);
                    } else {
                        mvaddch(screeny, screenx, 'L'); 
                    }       
                    attroff(COLOR_PAIR(MOUNTAIN_1_PAIR));
                } else if (c == 'M') {
                    attron(COLOR_PAIR(MOUNTAIN_2_PAIR));
                    if (x == player->player_x && y == player->player_y) {   
                        mvaddwstr(screeny, screenx, star);
                    } else {
                        mvaddch(screeny, screenx, 'M'); 
                    }
                    attroff(COLOR_PAIR(MOUNTAIN_2_PAIR));
                } else if (c == 'H') {
                    attron(COLOR_PAIR(MOUNTAIN_3_PAIR));
                    if (x == player->player_x && y == player->player_y) {   
                        mvaddwstr(screeny, screenx, star);
                    } else {
                        mvaddch(screeny, screenx, 'H'); 
                    }
                    attroff(COLOR_PAIR(MOUNTAIN_3_PAIR));
                } else if (c == '^') {
                    attron(COLOR_PAIR(MOUNTAIN_4_PAIR));
                    if (x == player->player_x && y == player->player_y) {   
                        mvaddwstr(screeny, screenx, star);
                    } else {
                        mvaddch(screeny, screenx, '^'); 
                    }
                    attroff(COLOR_PAIR(MOUNTAIN_4_PAIR));
                } else if (c == '+') {
                    attron(COLOR_PAIR(SNOW_PAIR));
                    if (x == player->player_x && y == player->player_y) {   
                        mvaddwstr(screeny, screenx, star);
                    } else {
                        mvaddch(screeny, screenx, '+'); 
                    }
                    attroff(COLOR_PAIR(SNOW_PAIR));
                } else
                {
                    attron(COLOR_PAIR(ERROR_PAIR));
                    if (x == player->player_x && y == player->player_y) {   
                        mvaddwstr(screeny, screenx, star);
                    } else {
                        mvaddch(screeny, screenx, 'E'); 
                    }
                    attroff(COLOR_PAIR(ERROR_PAIR));
                }
            }
        }
    }
    wrefresh(stdscr);
}

void Move_Player(int max_x, int max_y, char map[max_x][max_y], Player* player, int* Gamestate, int* display_x, int* display_y, int displaysize_x, int displaysize_y)
{
    // Move player
    flushinp();
    bool valid_move = false;
    while (valid_move == false)
    {
    int action = getch();
    if (action == KEY_DOWN && player->player_y + 1 < max_y) {
        player->player_y += 1;
        valid_move = true;
    } else if (action == KEY_UP && player->player_y - 1 >= 0) {
        player->player_y -= 1;
        valid_move = true;
    } else if (action == KEY_LEFT && player->player_x - 1 >= 0) {
        player->player_x -= 1;
        valid_move = true;
    } else if (action == KEY_RIGHT && player->player_x + 2 < max_x) {
        player->player_x += 1;
        valid_move = true;
    }
    }
    *display_y = player->player_y - displaysize_y / 2;
    *display_x = player->player_x - displaysize_x / 2;
    
    if (*display_x < 0) {
        *display_x = 0;
    } else if (*display_x > max_x - displaysize_x) {
        *display_x = max_x - displaysize_x;
    }

    if (*display_y < 0) {
        *display_y = 0;
    } else if (*display_y > max_y - displaysize_y) {
        *display_y = max_y - displaysize_y;
    }
    

    int battlechance = 0;
    switch (map[player->player_x][player->player_y])
    {
    case '~':
        battlechance = 0;
        break;
    case '-':
        battlechance = 0;
        break;
    case ' ':
        battlechance = 0;
        break;
    case 'L':
        battlechance = 0;
        break;
    case 'M':
        battlechance = 0;
        break;
    case 'H':
        battlechance = 0;
        break;
    case '^':
        battlechance = 0;
        break;
    case '+':
        battlechance = 0;
        break;
    }
    int battleRNG = get_random_number(0, 100);   
    if (battleRNG < battlechance)
    {
        *Gamestate = 2;
    }
    usleep(50000);
    Display_Map(max_x, max_y, map, player, *display_x, *display_y, displaysize_x, displaysize_y);

    
}