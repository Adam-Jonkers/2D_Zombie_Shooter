typedef struct {
    int Class;
    int Hp;
    int Attack;
    int Defense;
} Player;

typedef struct {
    int Class;
    int Hp;
    int Attack;
    int Defense;
} Enemy;

// gets random number
int get_random_number(int min, int max)
{
    return rand() % (max - min + 1) + min;
}

def calculate_damage(attacker, defender):
    damage = attacker.Attack - defender.Defense
    if damage < 0:
        damage = 0
    return damage

def StartBattle(player, enemy):
    while player.Hp > 0 and enemy.Hp > 0:
        player.Hp -= calculate_damage(enemy, player)
        enemy.Hp -= calculate_damage(player, enemy)
    if player.Hp <= 0:
        print("You lose!")
    else:
        print("You win!")