#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

#define PLAYER_ATTACKING true
#define PLAYER_DEFENDING false

#define MAX_CLASS_NAMR_LENGTH 16

#define MAP_WIDTH 80
#define MAP_HEIGHT 20

typedef struct {
    char Class[MAX_CLASS_NAMR_LENGTH];
    char Weapon[MAX_CLASS_NAMR_LENGTH];
    int Level;
    int MaxHp;
    int Hp;
    int Attack;
    int Defense;
    int player_x;
    int player_y;
} Player;

typedef struct {
    char Class[MAX_CLASS_NAMR_LENGTH];
    char Weapon[MAX_CLASS_NAMR_LENGTH];
    int Difficulty;
    int Hp;
    int Attack;
    int Defense;
} Enemy;

int get_random_number(int min, int max)
{
    return (rand() % (max - min + 1) + min);
}

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
        printf("Invalid class \n Please enter 1, 2 or 3\n");
        break;
    }
    }
    printf("You are a %s with a %s\n\n", player.Class, player.Weapon);
    player.player_x = get_random_number(1, MAP_WIDTH - 2);
    player.player_y = get_random_number(1, MAP_HEIGHT - 2);
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

void Setup_Map(char map[MAP_HEIGHT][MAP_WIDTH])
{
    printf("Generating map...\n\n");
    // Setup map
    for (int y = 0; y < MAP_HEIGHT; y++) {
        for (int x = 0; x < MAP_WIDTH; x++) {
            if (y == 0 || y == MAP_HEIGHT - 1 || x == 0 || x == MAP_WIDTH - 1) {
                map[y][x] = '#';
            } else if (y % 2 == 0 && x % 2 == 0) {
                map[y][x] = 'O';
            } else {
                map[y][x] = '.';
            }
        }
    }
}

void Display_Map(char map[MAP_HEIGHT][MAP_WIDTH])
{
    // Display map
    for (int y = 0; y < MAP_HEIGHT; y++) {
        for (int x = 0; x < MAP_WIDTH; x++) {
            printf("%c", map[y][x]);
        }
        printf("\n");
    }
}

int main()
{
    // Seed random number generator
    srand(time(NULL));

    // Setup map
    char map[MAP_HEIGHT][MAP_WIDTH];
    Setup_Map(map);

    // Setup player
    Player player = Setup_Player();

    // Place player on map
    map[player.player_y][player.player_x] = '@';

    // Display map
    Display_Map(map);

    // Start battle
    bool won = battle(&player);
    if (won == false)
    {
        printf("You Lost!\n");
        // Restart game
    }

}