#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <ncurses.h>

#include "CHARACTERS.h"
#include "CORE.h"

Player Setup_Player()
{
    bool valid_class = false;
    Player player;
    player.Level = 1;
    while (valid_class == false)
    {
    printw("Choose your class: \n1. Warrior \n2. Mage \n3. Rogue\n");
    refresh();
    int class = getch();
    printw("\nYou chose %c, \n", class);
    refresh();
    switch (class - '0')
    {
    case 1:
        strcpy(player.Class, "Warrior");
        strcpy(player.Weapon, "Sword");
        player.MaxHp = 100;
        player.Hp = 100;
        player.Attack = 10;
        player.Defense = 15;
        valid_class = true;
        break;
    
    case 2:
        strcpy(player.Class, "Mage");
        strcpy(player.Weapon, "Staff");
        player.MaxHp = 100;
        player.Hp = 100;
        player.Attack = 5;
        player.Defense = 10;
        valid_class = true;
        break;
    
    case 3:
        strcpy(player.Class, "Rogue");
        strcpy(player.Weapon, "Dagger");
        player.MaxHp = 100;
        player.Hp = 100;
        player.Attack = 7;
        player.Defense = 5;
        valid_class = true;
        break;

    default:
        //wclear(stdscr);
        printw("Invalid class \n");
        refresh();
        getch();
        wclear(stdscr);
        refresh();
        break;
    }
    }
    printw("You are a %s with a %s \nPRESS ANY KEY TO CONTINUE\n\n", player.Class, player.Weapon);
    refresh();
    getch();
    return player;
}

Enemy Setup_Enemy()
{
    Enemy enemy;
    int enemy_class = get_random_number(1, 3);
    switch (enemy_class)
    {
    case 1:
        strcpy(enemy.Class, "Warrior");
        strcpy(enemy.Weapon, "Sword");
        enemy.Hp = 100/2;
        enemy.Attack = 10/2;
        enemy.Defense = get_random_number(5, 15);
        break;
    
    case 2:
        strcpy(enemy.Class, "Mage");
        strcpy(enemy.Weapon, "Staff");
        enemy.Hp = 100/2;
        enemy.Attack = 5/2;
        enemy.Defense = get_random_number(0, 10);
        break;
    
    case 3:
        strcpy(enemy.Class, "Rogue");
        strcpy(enemy.Weapon, "Dagger");
        enemy.Hp = 100/2;
        enemy.Attack = 7/2;
        enemy.Defense = get_random_number(0, 5);
        break;
    }
    return enemy;
}