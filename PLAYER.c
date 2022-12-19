#define MAX_CLASS_NAMR_LENGTH 16

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