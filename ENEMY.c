#define MAX_CLASS_NAMR_LENGTH 16

typedef struct {
    char Class[MAX_CLASS_NAMR_LENGTH];
    char Weapon[MAX_CLASS_NAMR_LENGTH];
    int Difficulty;
    int Hp;
    int Attack;
    int Defense;
} Enemy;

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