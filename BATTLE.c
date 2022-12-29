#include <stdio.h>
#include <unistd.h>

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
            //printw("The enemy blocked your attack!\n\n");
            damage = 0;
        } else {
            damage = player.Attack;
            //printw("You dealt %d damage\n\n", damage);
        }
    } else {
        if (block_chance < player.Defense) {
            //printw("You blocked the enemy's attack!\n\n");
            damage = 0;
        } else {
            damage = enemy.Attack;
            //printw("The enemy dealt %d damage\n\n", damage);
        }
    }
    if (damage < 0) { 
        damage = 0;
    }
    return damage;
}

bool battle(Player* player)
{
    Enemy enemy = Setup_Enemy();
    //printw("You are fighting a %s with a %s\n\n", enemy.Class, enemy.Weapon);
    while (player->Hp > 0 && enemy.Hp > 0)
    {
        //printw("You have %d HP left\n", player->Hp);
        //printw("The enemy has %d HP left\n", enemy.Hp);
        //printw("What do you want to do? \n1. Attack \n2. Defend\n\n");
        char action = 1; //getch();
        //printw("\nYou chose %c, \n", action);
        switch (action - '0')
        {
        case 1:
            enemy.Hp -= calculate_damage(*player, enemy, PLAYER_ATTACKING);
            player->Hp -= calculate_damage(*player, enemy, PLAYER_DEFENDING);
            sleep(2);
            break;
        
        case 2:
            player->Defense += 40;
            player->Hp -= calculate_damage(*player, enemy, PLAYER_DEFENDING);
            player->Defense -= 40;
            sleep(2);
            break;
        
        default:
            //printw("Invalid action \n");
            sleep(2);
            break;
        }
    }
    if (player->Hp > 0)
    {
        //printw("You won!\n");
        return true;
    }
    else
    {
        //printw("You lost!\n");
        return false;
    }
}