#include <stdio.h>
#include <ncurses.h>

#include "BATTLE.h"
#include "CHARACTERS.h"
#include "CORE.h"

#define PLAYER_ATTACKING true
#define PLAYER_DEFENDING false

int calculate_damage(Player player, Enemy enemy, bool Attacking)
{
    int damage;
    int block_chance = get_random_number(0, 100);
    if (Attacking == true)
    {
        if (block_chance < enemy.Defense) {
            printw("The enemy blocked your attack!\n\n");
            wrefresh(stdscr);
            damage = 0;
        } else {
            damage = player.Attack;
            printw("You dealt %d damage\n\n", damage);
            wrefresh(stdscr);
        }
    } else {
        if (block_chance < player.Defense) {
            printw("You blocked the enemy's attack!\n\n");
            wrefresh(stdscr);
            damage = 0;
        } else {
            damage = enemy.Attack;
            printw("The enemy dealt %d damage\n\n", damage);
            wrefresh(stdscr);
        }
    }
    if (damage < 0) { 
        damage = 0;
    }
    return damage;
}

bool battle(Player* player)
{
    wclear(stdscr);
    Enemy enemy = Setup_Enemy();
    printw("You are fighting a %s with a %s\n\n", enemy.Class, enemy.Weapon);
    while (player->Hp > 0 && enemy.Hp > 0)
    {
        printw("You have %d HP left\n", player->Hp);
        printw("The enemy has %d HP left\n", enemy.Hp);
        printw("What do you want to do? \n1. Attack \n2. Defend\n\n");
        wrefresh(stdscr);
        char action = getch();
        printf("\nYou chose %c, \n", action);
        switch (action - '0')
        {
        case 1:
            enemy.Hp -= calculate_damage(*player, enemy, PLAYER_ATTACKING);
            player->Hp -= calculate_damage(*player, enemy, PLAYER_DEFENDING);
            getch();
            break;
        
        case 2:
            player->Defense += 25;
            player->Hp -= calculate_damage(*player, enemy, PLAYER_DEFENDING);
            player->Defense -= 25;
            getch();
            break;
        
        default:
            printw("Invalid action \n");
            wrefresh(stdscr);
            getch();
            break;
        }
        wclear(stdscr);
    }
    if (player->Hp > 0)
    {
        printw("You won!\n");
        wrefresh(stdscr);
        return true;
    }
    else
    {
        printw("You lost!\n");
        wrefresh(stdscr);
        return false;
    }
}