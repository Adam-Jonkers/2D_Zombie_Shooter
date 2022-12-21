#include <stdio.h>

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
            printf("The enemy blocked your attack!\n\n");
            damage = 0;
        } else {
            damage = player.Attack;
            printf("You dealt %d damage\n\n", damage);
        }
    } else {
        if (block_chance < player.Defense) {
            printf("You blocked the enemy's attack!\n\n");
            damage = 0;
        } else {
            damage = enemy.Attack;
            printf("The enemy dealt %d damage\n\n", damage);
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
    printf("You are fighting a %s with a %s\n\n", enemy.Class, enemy.Weapon);
    while (player->Hp > 0 && enemy.Hp > 0)
    {
        printf("You have %d HP left\n", player->Hp);
        printf("The enemy has %d HP left\n", enemy.Hp);
        printf("What do you want to do? \n1. Attack \n2. Defend\n\n");
        int c;
        while ((c = getchar()) != '\n' && c != EOF) 
        { 
            continue;
        }
        char action = getchar();
        printf("\nYou chose %c, \n", action);
        switch (action - '0')
        {
        case 1:
            enemy.Hp -= calculate_damage(*player, enemy, PLAYER_ATTACKING);
            player->Hp -= calculate_damage(*player, enemy, PLAYER_DEFENDING);
            break;
        
        case 2:
            player->Defense += 25;
            player->Hp -= calculate_damage(*player, enemy, PLAYER_DEFENDING);
            player->Defense -= 25;
            break;
        
        default:
            printf("Invalid action \n Please enter 1 or 2\n\n");
            break;
        }
    }
    if (player->Hp > 0)
    {
        printf("You won!\n");
        return true;
    }
    else
    {
        printf("You lost!\n");
        return false;
    }
}