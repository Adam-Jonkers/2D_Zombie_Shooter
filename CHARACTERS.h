#ifndef CHARACTERS_H
#define CHARACTERS_H

#define MAX_NAME_LENGTH 20

typedef struct Player {
    char Class[MAX_NAME_LENGTH];
    char Weapon[MAX_NAME_LENGTH];
    int Level;
    int MaxHp;
    int Hp;
    int Attack;
    int Defense;
    int player_x;
    int player_y;
} Player;

typedef struct Enemy {
    char Class[MAX_NAME_LENGTH];
    char Weapon[MAX_NAME_LENGTH];
    int Difficulty;
    int Hp;
    int Attack;
    int Defense;
} Enemy;

Player Setup_Player(void);

Enemy Setup_Enemy(void);

#endif
