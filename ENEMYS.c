#include "ENEMYS.h"
#include "PLAYER.h"
#include "CORE.h"

void Setup_enemy(Enemys_t* enemys, vec2_t windowSize, SDL_Renderer* renderer)
{
    void* ptr = realloc(enemys->enemy, (enemys->num_enemys + 1) * sizeof(Enemy_t*));
    if (ptr == NULL) {
        printf("Error: Failed to allocate memory for bullet\n");
    } else {
        enemys->enemy = ptr;
    }

    enemys->enemy[enemys->num_enemys] = calloc(1, sizeof(Enemy_t));

    enemys->enemy[enemys->num_enemys]->texture = load_texture("Assets/Zombie/Zombie.png", renderer);

    int w, h;

    if (SDL_QueryTexture(enemys->enemy[enemys->num_enemys]->texture, NULL, NULL, &w, &h)) {
        printf("Error: %s Failed to query texture\n", SDL_GetError());
    }

    enemys->enemy[enemys->num_enemys]->health = 100;
    enemys->enemy[enemys->num_enemys]->damage = 10;

    enemys->enemy[enemys->num_enemys]->sprite.w = w/3.5;
    enemys->enemy[enemys->num_enemys]->sprite.h = h/3.5;
    enemys->enemy[enemys->num_enemys]->sprite.x = windowSize.x / 2 - enemys->enemy[enemys->num_enemys]->sprite.w / 2;
    enemys->enemy[enemys->num_enemys]->sprite.y = windowSize.y / 2 - enemys->enemy[enemys->num_enemys]->sprite.h / 2;
    enemys->enemy[enemys->num_enemys]->center.x = 95/3.5;
    enemys->enemy[enemys->num_enemys]->center.y = 120/3.5;
    enemys->enemy[enemys->num_enemys]->rotation = 0.0;
    enemys->enemy[enemys->num_enemys]->maxSpeed = 100.0;
    enemys->enemy[enemys->num_enemys]->position.x = 100;
    enemys->enemy[enemys->num_enemys]->position.y = 100;
    enemys->enemy[enemys->num_enemys]->velocity.x = 0.0;
    enemys->enemy[enemys->num_enemys]->velocity.y = 0.0;
    enemys->enemy[enemys->num_enemys]->acceleration = 20.0;

    printf("enemy setup complete\n");
}

void Create_enemy(Enemys_t* enemys, vec2_t windowSize, SDL_Renderer* renderer)
{
    printf("Creating enemy\n");
    void* ptr = realloc(enemys->enemy, (enemys->num_enemys + 1) * sizeof(Enemy_t*));
    if (ptr == NULL) {
        printf("Error: Failed to allocate memory for bullet\n");
    } else {
        enemys->enemy = ptr;
    }

    enemys->enemy[enemys->num_enemys] = calloc(1, sizeof(Enemy_t));

    Setup_enemy(enemys, windowSize, renderer);
}

void Draw_Enemy(SDL_Renderer* renderer, Enemy_t* enemy, vec2_t windowSize, Player_t* player)
{
    enemy->sprite.x = enemy->position.x - enemy->sprite.w / 2 - player->camera.x;
    enemy->sprite.y = enemy->position.y - enemy->sprite.h / 2 - player->camera.y;

    if (enemy->position.x > player->camera.x - 100 && enemy->position.x < player->camera.x + windowSize.x + 100 && enemy->position.y > player->camera.y - 100 && enemy->position.y < player->camera.y + windowSize.y + 100) {
        if (SDL_RenderCopyExF(renderer, enemy->texture, NULL, &enemy->sprite, enemy->rotation, &enemy->center, SDL_FLIP_NONE) == -1) {
            printf("Error: %s Failed to draw enemy\n", SDL_GetError());
        } else {
            printf("Drawn enemy x:%f y:%f player x:%f y:%f\n", enemy->position.x, enemy->position.y, player->position.x, player->position.y);
        }
    }
}

void Draw_Enemys(SDL_Renderer* renderer, Enemys_t* enemys, vec2_t windowSize, Player_t* player)
{
    for (int i = 0; i < enemys->num_enemys; i++) {
        Draw_Enemy(renderer, enemys->enemy[i], windowSize, player);
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
//     enemy->velocity = vec2_limit(enemy->velocity, enemy->maxSpeed);
//     enemy->position = vec2_add(enemy->position, vec2_mul(enemy->velocity, dt));
    enemy->rotation = get_angle(enemy->position, player->position) + dt * 0;
}

void Update_Enemys(Enemys_t* enemys, Player_t* player, float dt)
{
    for (int i = 0; i < enemys->num_enemys; i++) {
        Update_Enemy(enemys->enemy[i], player, dt);
    }
}