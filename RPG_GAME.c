#include <stdio.h>
#include <stdbool.h>

typedef struct {
    // 1: Warrior, 2: Mage, 3: Rogue
    int Class;
    // 1: Sword, 2: Staff, 3: Dagger
    int Weapon;
    int Level;
    int MaxHp;
    int Hp;
    int Attack;
    int Defense;
} Player;

typedef struct {
    // 1: Warrior, 2: Mage, 3: Rogue
    int Class;
    // 1: Sword, 2: Staff, 3: Dagger
    int Weapon;
    int Difficulty;
    int Hp;
    int Attack;
    int Defense;
} Enemy;

int get_random_number(int min, int max)
{
    return (rand() % (max - min + 1) + min);
}

int calculate_damage(attacker, defender)
{
    damage = attacker.Attack - defender.Defense
    if (damage < 0) { 
        damage = 0;
    }
    return damage
}

Player Setup_Player()
{
    bool valid_class = false;
    Player player;
    player.Level = 1;
    while (valid_class == false)
    {
    printf("Choose your class: 1. Warrior \n2. Mage \n3. Rogue");
    int class = getchar();
    switch (class)
    {
    case 1:
        player.Class = "Warrior";
        player.Weapon = "Sword";
        player.MaxHp = 100;
        player.Hp = 100;
        player.Attack = 10;
        player.Defense = 5;
        valid_class = true;
        break;
    
    case 2:
        player.Class = "Mage";
        player.Weapon = "Staff";
        player.MaxHp = 100;
        player.Hp = 100;
        player.Attack = 5;
        player.Defense = 10;
        valid_class = true;
        break;
    
    case 3:
        player.Class = "Rogue";
        player.Weapon = "Dagger";
        player.MaxHp = 100;
        player.Hp = 100;
        player.Attack = 7;
        player.Defense = 7;
        valid_class = true;
        break;

    default:
        printf("Invalid class \n Please enter 1, 2 or 3");
        break;
    }
    }
    return player
}

Enemy Setup_Enemy()
{
    Enemy enemy;
    int enemy_class = get_random_number(1, 3);
    switch (enemy_class)
    {
    case 1:
        enemy.Class = "Warrior";
        enemy.Weapon = "Sword";
        enemy.Hp = 100/2;
        enemy.Attack = 10/2;
        enemy.Defense = 5/2;
        break;
    
    case 2:
        enemy.Class = "Mage";
        enemy.Weapon = "Staff";
        enemy.Hp = 100/2;
        enemy.Attack = 5/2;
        enemy.Defense = 10/2;
        break;
    
    case 3:
        enemy.Class = "Rogue";
        enemy.Weapon = "Dagger";
        enemy.Hp = 100/2;
        enemy.Attack = 7/2;
        enemy.Defense = 7/2;
        break;
    }
    return enemy
}