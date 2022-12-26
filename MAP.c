#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <ncurses.h>

#include "MAP.h"
#include "CHARACTERS.h"
#include "CORE.h"

#define PLAYER_PAIR 9

void generate_texture_map(int max_x, int max_y, float noisemap[max_x][max_y]) {
    for (int x = 0; x < max_x; x++) {
        for (int y = 0; y < max_y; y++)
        {
            noisemap[x][y] = (float)rand() / RAND_MAX;
        } 
    }
}

float fade(float t) {
    return t*t*t*(t*(t*6.0-15.0)+10.0);
}

vec2 grad(vec2 p, int max_x, int max_y, float noisemap[max_x][max_y]) {
    vec2 v;
    if (p.x < (max_x - 1) && p.y < (max_y - 1))
    {
    v.x = noisemap[(int)p.x][(int)p.y] - noisemap[((int)p.x) + 1][(int)p.y];
    v.y = noisemap[(int)p.x][(int)p.y] - noisemap[(int)p.x][((int)p.y) + 1];
    }
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

void Setup_Noise_Map(int max_x, int max_y, float noisemap[max_x][max_y])
{
    wclear(stdscr);
    printw("Generating noise map...\n\n");
    wrefresh(stdscr);
    // Setup noise map
    generate_texture_map(max_x, max_y, noisemap);
    for (int y = 0; y < max_y; y++)
    {
        for (int x = 0; x < max_x; x++)
        {
            vec2 i = (vec2){x, y};
            float n = noise(divide_vec2(i, 64), max_x, max_y, noisemap) * 1.0 +
            noise(divide_vec2(i, 32), max_x, max_y, noisemap) * 0.5 +
            noise(divide_vec2(i, 16), max_x, max_y, noisemap) * 0.25 +
            noise(divide_vec2(i, 8), max_x, max_y, noisemap) * 0.125;
            noisemap[x][y] = n;
        }
    }
    wrefresh(stdscr);
}

void Setup_Map(int max_x, int max_y ,char map[max_x][max_y], float noisemap[max_x][max_y], Player* player)
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
            } else if (n < 0.80) {
                map[x][y] = '+';
            }
        }
    }
    // Place player on map
    player->player_x = get_random_number(1, max_x - 2);
    player->player_y = get_random_number(1, max_y - 2);
}

void Display_Map(int max_x, int max_y, char map[max_x][max_y], Player* player)
{
    wclear(stdscr);
    wrefresh(stdscr);
    // Display map
    for (int y = 0; y < max_y; y++)
    {
        for (int x = 0; x < max_x; x++)
        {
            if (x == player->player_x && y == player->player_y)
            {
                attron(COLOR_PAIR(9));
                wprintw(stdscr, "P");
                attroff(COLOR_PAIR(9));
            }
            else
            {
                char c = map[x][y];
                if (c == '~') {
                    attron(COLOR_PAIR(1));
                    wprintw(stdscr, "~");
                    attroff(COLOR_PAIR(1));
                } else if (c == '-') {
                    attron(COLOR_PAIR(2));
                    wprintw(stdscr, "-");
                    attroff(COLOR_PAIR(2));
                } else if (c == ' ') {
                    attron(COLOR_PAIR(3));
                    wprintw(stdscr, " ");
                    attroff(COLOR_PAIR(3));
                } else if (c == 'L') {
                    attron(COLOR_PAIR(4));
                    wprintw(stdscr, "L");
                    attroff(COLOR_PAIR(4));
                } else if (c == 'M') {
                    attron(COLOR_PAIR(5));
                    wprintw(stdscr, "M");
                    attroff(COLOR_PAIR(5));
                } else if (c == 'H') {
                    attron(COLOR_PAIR(6));
                    wprintw(stdscr, "H");
                    attroff(COLOR_PAIR(6));
                } else if (c == '^') {
                    attron(COLOR_PAIR(7));
                    wprintw(stdscr, "^");
                    attroff(COLOR_PAIR(7));
                } else if (c == '+') {
                    attron(COLOR_PAIR(8));
                    wprintw(stdscr, "+");
                    attroff(COLOR_PAIR(8));
                }
            }
        }
        wprintw(stdscr, "\n");
    }
    wrefresh(stdscr);
}

void Move_Player(int max_x, int max_y, char map[max_x][max_y], Player* player)
{
    // Move player
    bool valid_move = false;
    while (valid_move == false)
    {
    int action = getch();
    if (action == KEY_DOWN && map[player->player_y - 1][player->player_x] != '#') {
        player->player_y += 1;
        valid_move = true;
    } else if (action == KEY_UP && map[player->player_y + 1][player->player_x] != '#') {
        player->player_y -= 1;
        valid_move = true;
    } else if (action == KEY_LEFT && map[player->player_y][player->player_x - 1] != '#') {
        player->player_x -= 1;
        valid_move = true;
    } else if (action == KEY_RIGHT && map[player->player_y][player->player_x + 1] != '#') {
        player->player_x += 1;
        valid_move = true;
    } else {
        printw("%c is an invalid move!\n", action);
        wrefresh(stdscr);
    }
    }
    Display_Map(max_x, max_y, map, player);
}