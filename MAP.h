#ifndef MAP_H
#define MAP_H

#include "CHARACTERS.h"

typedef struct Specialization {
    char type[20];
    int friendly_chance;
} Specialization;

typedef struct Shop {
    int x;
    int y;
    char name[20];
    int level;
    int gold;
    int items;
    char map_icon;
} Shop;

typedef struct Road {
    int x;
    int y;
    int battle_chance;
    char map_icon;
} Road;

typedef struct Forest {
    int x;
    int y;
    int battle_chance;
    char map_icon;
} Forest;

typedef struct Mountain {
    int x;
    int y;
    int battle_chance;
    char map_icon;
} Mountain;

typedef struct River {
    int x;
    int y;
    int battle_chance;
    char map_icon;
} River;

typedef struct Lake {
    int x;
    int y;
    int battle_chance;
    char map_icon;
} Lake;

typedef struct Swamp {
    int x;
    int y;
    int battle_chance;
    char map_icon;
} Swamp;

typedef struct Cave {
    int x;
    int y;
    int battle_chance;
    char map_icon;
} Cave;

typedef struct Desert {
    int x;
    int y;
    int battle_chance;
    char map_icon;
} Desert;

typedef struct Plains {
    int x;
    int y;
    int battle_chance;
    char map_icon;
} Plains;

typedef struct Town {
    int x;
    int y;
    char name[20];
    int population;
    int level;
    Specialization specialization;
    bool shop;
    char map_icon;
} Town;

typedef struct Dungeon {
    int x;
    int y;
    char name[20];
    int level;
    int difficulty;
    char map_icon;
} Dungeon;

typedef struct village {
    int x;
    int y;
    char name[20];
    int population;
    int level;
    char map_icon;
} Village;

typedef struct castle {
    int x;
    int y;
    char name[20];
    int level;
    int difficulty;
    bool quest;
    bool fight;
    char map_icon;
} Castle;

typedef struct Clearing {
    int x;
    int y;
    char name[20];
    int level;
    int difficulty;
    bool quest;
    bool fight;
    char map_icon;
} Clearing;

void Setup_Map(int map_width, int map_height, char map[map_height][map_width], Player* player);

void Display_Map(int map_width, int map_height, char map[map_height][map_width]);

#endif