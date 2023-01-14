#include "ENEMYS.h"
#include "PLAYER.h"
#include "MAP.h"
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
    enemys->enemy[enemys->num_enemys]->maxSpeed = 150.0;
    enemys->enemy[enemys->num_enemys]->position.x = 100;
    enemys->enemy[enemys->num_enemys]->position.y = 100;
    enemys->enemy[enemys->num_enemys]->velocity.x = 0.0;
    enemys->enemy[enemys->num_enemys]->velocity.y = 0.0;
    enemys->enemy[enemys->num_enemys]->acceleration = 20.0;
    enemys->enemy[enemys->num_enemys]->hitBox.h = enemys->enemy[enemys->num_enemys]->sprite.h * 0.8;
    enemys->enemy[enemys->num_enemys]->hitBox.w = enemys->enemy[enemys->num_enemys]->sprite.w * 0.8;

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
        if (SDL_RenderCopyExF(renderer, enemy->texture, NULL, &enemy->sprite, enemy->rotation * (180 / M_PI), NULL, SDL_FLIP_NONE) == -1) {
            printf("Error: %s Failed to draw enemy\n", SDL_GetError());
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

void Update_Enemy(Enemy_t* enemy, Player_t* player, float dt, vec2_t max, float* noiseMap)
{
    enemy->rotation = get_angle(enemy->position, player->position) - 1.5708;
    
    enemy->velocity.x += enemy->acceleration * cos(enemy->rotation + 1.5708);
    enemy->velocity.y += enemy->acceleration * sin(enemy->rotation + 1.5708);

    float speedMultiplier = Get_speed_multiplier(enemy->position, max, noiseMap);
    if (length_vec2(enemy->velocity) > (enemy->maxSpeed * speedMultiplier)) {
        enemy->velocity = normalize_vec2(enemy->velocity);
        enemy->velocity = multiply_vec2(enemy->velocity, enemy->maxSpeed * speedMultiplier);
    }

    vec2_t tempPosition = add_vec2(enemy->position, divide_vec2(multiply_vec2(enemy->velocity, dt), 1000.0f));
    if (tempPosition.x < 0 || tempPosition.x > max.x) {
        enemy->velocity.x = 0;
    }
    if (tempPosition.y < 0 || tempPosition.y > max.y) {
        enemy->velocity.y = 0;
    }
    enemy->position = add_vec2(enemy->position, divide_vec2(multiply_vec2(enemy->velocity, dt), 1000.0f));
}

void Update_Enemys(Enemys_t* enemys, Player_t* player, float dt, vec2_t max, float* noiseMap)
{
    for (int i = 0; i < enemys->num_enemys; i++) {
        Update_Enemy(enemys->enemy[i], player, dt, max, noiseMap);
    }
}