#ifndef BATTLE_H
#define BATTLE_H

#include <stdbool.h>

#include "CHARACTERS.h"

int calculate_damage(Player player, Enemy enemy, bool Attacking);

bool battle(Player* player);

#endif