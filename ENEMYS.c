#include "ENEMYS.h"
#include "PLAYER.h"
#include "CORE.h"

Enemy_t* Setup_enemy(vec2_t windowsize, SDL_Renderer* renderer)
{
    Enemy_t* enemy;

    enemy = calloc(1, sizeof(Enemy_t));

    load_texture("Assets/Zombie/Zombie.png", renderer);

    int w, h;

    SDL_QueryTexture(enemy->texture, NULL, NULL, &w, &h);

    enemy->health = 100;
    enemy->damage = 10;

    enemy->sprite.w = w/3.5;
    enemy->sprite.h = h/3.5;
    enemy->sprite.x = windowsize.x / 2 - enemy->sprite.w / 2;
    enemy->sprite.y = windowsize.y / 2 - enemy->sprite.h / 2;
    enemy->center.x = 95/3.5;
    enemy->center.y = 120/3.5;
    enemy->rotation = 0.0;
    enemy->maxspeed = 100.0;
    enemy->position.x = 1200;
    enemy->position.y = 1200;
    enemy->velocity.x = 0.0;
    enemy->velocity.y = 0.0;
    enemy->acceleration = 20.0;

    printf("enemy setup complete\n");

    return enemy;
}

void Draw_Enemy(SDL_Renderer* renderer, Enemy_t* enemy, vec2_t windowsize, Player_t* player)
{
    enemy->sprite.x = enemy->position.x - enemy->sprite.w / 2;
    enemy->sprite.y = enemy->position.y - enemy->sprite.h / 2;

    if (enemy->position.x > player->camera.x - 100 && enemy->position.x < player->camera.x + windowsize.x + 100 && enemy->position.y > player->camera.y - 100 && enemy->position.y < player->camera.y + windowsize.y + 100) {

    }
    SDL_RenderCopyExF(renderer, enemy->texture, NULL, &enemy->sprite, enemy->rotation, &enemy->center, SDL_FLIP_NONE);
}

void Draw_Enemys(SDL_Renderer* renderer, Enemy_t** enemy, int num_enemys, vec2_t windowsize, Player_t* player)
{
    for (int i = 0; i < num_enemys; i++) {
        Draw_Enemy(renderer, enemy[i], windowsize, player);
    }
}

void Destroy_Enemy(Enemy_t* enemy)
{
    SDL_DestroyTexture(enemy->texture);
    free(enemy);
}

void Remove_Enemy(Enemys_t* enemys, int index)
{
    if (index < 0 || index >= enemys->num_enemys) {
        return;
    }

    Destroy_Enemy(enemys->enemy[index]);

    for (int i = index; i < enemys->num_enemys - 1; i++) {
        enemys->enemy[i] = enemys->enemy[i + 1];
        enemys->enemy[i]->index--;
    }

    enemys->num_enemys--;

    if (enemys->num_enemys == 0) {
        free(enemys->enemy);
        enemys->enemy = NULL;
        return;
    } else {
        void* ptr = realloc(enemys->enemy, enemys->num_enemys * sizeof(Enemy_t*));
        if (ptr == NULL) {
            printf("Error: Failed to allocate memory for removing enemy\n");
        } else {
            enemys->enemy = ptr;
        }
    }
}

void Remove_Enemys(Enemys_t* enemys)
{
    for (int i = 0; i < enemys->num_enemys; i++) {
        Remove_Enemy(enemys, i);
    }
}

void Update_Enemy(Enemy_t* enemy, Player_t* player, float dt)
{
//     vec2_t direction = vec2_sub(player->position, enemy->position);
//     vec2_t normalized = vec2_normalize(direction);
//     vec2_t force = vec2_mul(normalized, enemy->acceleration);
//     enemy->velocity = vec2_add(enemy->velocity, force);
//     enemy->velocity = vec2_limit(enemy->velocity, enemy->maxspeed);
//     enemy->position = vec2_add(enemy->position, vec2_mul(enemy->velocity, dt));
//     enemy->rotation = vec2_angle(enemy->velocity) * 180 / M_PI;
}

void Update_Enemys(Enemys_t* enemys, Player_t* player, float dt)
{
    for (int i = 0; i < enemys->num_enemys; i++) {
        Update_Enemy(enemys->enemy[i], player, dt);
    }
}