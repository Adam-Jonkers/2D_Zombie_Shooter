#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <time.h>

#define PLAYER_ATTACKING true
#define PLAYER_DEFENDING false

#define Warrior 1
#define Mage 2
#define Rogue 3

#define Sword 1
#define Staff 2
#define Dagger 3

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
    srand ( time(NULL) );
    return (rand() % (max - min + 1) + min);
}

int calculate_damage(Player player, Enemy enemy, bool Attacking)
{
    int damage;
    if (Attacking == true)
    {
        damage = player.Attack - enemy.Defense;
    }
    else
    {
        damage = enemy.Attack - player.Defense;
    }
    if (damage < 0) { 
        damage = 0;
    }
    return damage;
}

Player Setup_Player()
{
    bool valid_class = false;
    Player player;
    player.Level = 1;
    while (valid_class == false)
    {
    printf("Choose your class: \n1. Warrior \n2. Mage \n3. Rogue\n\n");
    int class = getchar();
    switch (class - '0')
    {
    case 1:
        player.Class = Warrior;
        player.Weapon = Sword;
        player.MaxHp = 100;
        player.Hp = 100;
        player.Attack = 10;
        player.Defense = 5;
        valid_class = true;
        break;
    
    case 2:
        player.Class = Mage;
        player.Weapon = Staff;
        player.MaxHp = 100;
        player.Hp = 100;
        player.Attack = 5;
        player.Defense = 10;
        valid_class = true;
        break;
    
    case 3:
        player.Class = Rogue;
        player.Weapon = Dagger;
        player.MaxHp = 100;
        player.Hp = 100;
        player.Attack = 7;
        player.Defense = 7;
        valid_class = true;
        break;

    default:
        printf("Invalid class \n Please enter 1, 2 or 3\n");
        break;
    }
    }
    return player;
}

Enemy Setup_Enemy()
{
    Enemy enemy;
    int enemy_class = get_random_number(1, 3);
    switch (enemy_class)
    {
    case 1:
        enemy.Class = Warrior;
        enemy.Weapon = Sword;
        enemy.Hp = 100/2;
        enemy.Attack = 10/2;
        enemy.Defense = 5/2;
        break;
    
    case 2:
        enemy.Class = Mage;
        enemy.Weapon = Staff;
        enemy.Hp = 100/2;
        enemy.Attack = 5/2;
        enemy.Defense = 10/2;
        break;
    
    case 3:
        enemy.Class = Rogue;
        enemy.Weapon = Dagger;
        enemy.Hp = 100/2;
        enemy.Attack = 7/2;
        enemy.Defense = 7/2;
        break;
    }
    return enemy;
}

bool battle(Player* player)
{
    Enemy enemy = Setup_Enemy();
    printf("You are fighting a %d with a %d\n\n", enemy.Class, enemy.Weapon);
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
            player->Defense += 5;
            player->Hp -= calculate_damage(*player, enemy, PLAYER_DEFENDING);
            player->Defense -= 5;
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

int main()
{
    Player player = Setup_Player();
    printf("You are a %d with a %d\n\n", player.Class, player.Weapon);
    bool won = battle(&player);
    if (won == false)
    {
        printf("You Lost!\n");
        // Restart game
    }

}