#include "ENEMYS.h"
#include "PLAYER.h"
#include "MAP.h"
#include "CORE.h"

// ms per spawn
#define SPAWN_RATE 2000 

void Setup_enemy(Enemys_t* enemys, vec2_t windowSize, SDL_Renderer* renderer, vec2_t max)
{
    void* ptr = realloc(enemys->enemy, (enemys->num_enemys + 1) * sizeof(Enemy_t*));
    if (ptr == NULL) {
        printf("Error: Failed to allocate memory for enemy pointer\n");
    } else {
        enemys->enemy = ptr;
    }

    enemys->enemy[enemys->num_enemys] = calloc(1, sizeof(Enemy_t));
    if (enemys->enemy[enemys->num_enemys] == NULL) {
        printf("Error: Failed to allocate memory for enemy\n");
    }

    enemys->enemy[enemys->num_enemys]->texture = load_texture("Assets/Zombie/Zombie.png", renderer);

    int w, h;

    if (SDL_QueryTexture(enemys->enemy[enemys->num_enemys]->texture, NULL, NULL, &w, &h)) {
        printf("Error: %s Failed to query texture\n", SDL_GetError());
    }

    enemys->enemy[enemys->num_enemys]->health = 100;
    enemys->enemy[enemys->num_enemys]->damage = 10;
    enemys->enemy[enemys->num_enemys]->attackRate = 1000;
    enemys->enemy[enemys->num_enemys]->attackRange =  100;

    enemys->enemy[enemys->num_enemys]->sprite.w = w/3.5;
    enemys->enemy[enemys->num_enemys]->sprite.h = h/3.5;
    enemys->enemy[enemys->num_enemys]->sprite.x = windowSize.x / 2 - enemys->enemy[enemys->num_enemys]->sprite.w / 2;
    enemys->enemy[enemys->num_enemys]->sprite.y = windowSize.y / 2 - enemys->enemy[enemys->num_enemys]->sprite.h / 2;
    enemys->enemy[enemys->num_enemys]->center.x = 95/3.5;
    enemys->enemy[enemys->num_enemys]->center.y = 120/3.5;
    enemys->enemy[enemys->num_enemys]->rotation = 0.0;
    enemys->enemy[enemys->num_enemys]->maxSpeed = (float)get_random_number(110, 190);
    enemys->enemy[enemys->num_enemys]->position.x = get_random_number(0 + 10, max.x - 10);
    enemys->enemy[enemys->num_enemys]->position.y = get_random_number(0 + 10, max.y - 10);
    enemys->enemy[enemys->num_enemys]->velocity.x = 0.0;
    enemys->enemy[enemys->num_enemys]->velocity.y = 0.0;
    enemys->enemy[enemys->num_enemys]->acceleration = (float)get_random_number(15, 25);
    enemys->enemy[enemys->num_enemys]->hitBox.h = enemys->enemy[enemys->num_enemys]->sprite.h * 0.8;
    enemys->enemy[enemys->num_enemys]->hitBox.w = enemys->enemy[enemys->num_enemys]->sprite.w * 0.8;
    enemys->enemy[enemys->num_enemys]->attackTimer = create_timer();
    start_timer(&enemys->enemy[enemys->num_enemys]->attackTimer);

    enemys->enemy[enemys->num_enemys]->index = enemys->num_enemys;

    enemys->num_enemys++;
}

void Create_enemy(Enemys_t* enemys, vec2_t windowSize, SDL_Renderer* renderer, vec2_t max)
{
    printf("Creating enemy\n");
    void* ptr = realloc(enemys->enemy, (enemys->num_enemys + 1) * sizeof(Enemy_t*));
    if (ptr == NULL) {
        printf("Error: Failed to allocate memory for enemy\n");
    } else {
        enemys->enemy = ptr;
    }

    enemys->enemy[enemys->num_enemys] = calloc(1, sizeof(Enemy_t));

    Setup_enemy(enemys, windowSize, renderer, max);
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
    if (index < 0 || index > enemys->num_enemys) {
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

void Update_Enemy(Enemys_t* enemys, Player_t* player, float dt, vec2_t max, float* noiseMap, int index, SDL_Renderer* renderer)
{
    enemys->enemy[index]->rotation = get_angle(enemys->enemy[index]->position, player->position) - 1.5708;
    
    enemys->enemy[index]->velocity.x += enemys->enemy[index]->acceleration * cos(enemys->enemy[index]->rotation + 1.5708);
    enemys->enemy[index]->velocity.y += enemys->enemy[index]->acceleration * sin(enemys->enemy[index]->rotation + 1.5708);

    float speedMultiplier = Get_speed_multiplier(enemys->enemy[index]->position, max, noiseMap);
    if (length_vec2(enemys->enemy[index]->velocity) > (enemys->enemy[index]->maxSpeed * speedMultiplier)) {
        enemys->enemy[index]->velocity = normalize_vec2(enemys->enemy[index]->velocity);
        enemys->enemy[index]->velocity = multiply_vec2(enemys->enemy[index]->velocity, enemys->enemy[index]->maxSpeed * speedMultiplier);
    }

    vec2_t tempPosition = add_vec2(enemys->enemy[index]->position, divide_vec2(multiply_vec2(enemys->enemy[index]->velocity, dt), 1000.0f));
    if (tempPosition.x < 0 || tempPosition.x > max.x) {
        enemys->enemy[index]->velocity.x = 0;
    }
    if (tempPosition.y < 0 || tempPosition.y > max.y) {
        enemys->enemy[index]->velocity.y = 0;
    }
    if (abs((int)Get_Distance(tempPosition, player->position) < 75)) {
        enemys->enemy[index]->velocity = (vec2_t){0, 0};
    }
    enemys->enemy[index]->position = add_vec2(enemys->enemy[index]->position, divide_vec2(multiply_vec2(enemys->enemy[index]->velocity, dt), 1000.0f));
    if (enemys->enemy[index]->health <= 0) {
        Remove_Enemy(enemys, index);
    }
    if (abs((int)Get_Distance(enemys->enemy[index]->position, player->position)) < enemys->enemy[index]->attackRange && get_time_ms(&enemys->enemy[index]->attackTimer) > enemys->enemy[index]->attackRate) {
        player->health -= 1;
        SDL_SetRenderDrawColor(renderer, 255, 0, 0, 75);
        SDL_RenderFillRect(renderer, NULL);
        start_timer(&enemys->enemy[index]->attackTimer);
    }
}

void Update_Enemys(Enemys_t* enemys, Player_t* player, float dt, vec2_t max, float* noiseMap, SDL_Renderer* renderer)
{
    if (enemys->num_enemys == -1) {
        return;
    }
    for (int i = 0; i < enemys->num_enemys; i++) {
        Update_Enemy(enemys, player, dt, max, noiseMap, i, renderer);
    }
}

void Spawn_Enemys(Enemys_t* enemys, vec2_t windowSize, SDL_Renderer* renderer, vec2_t max, Timer_t *timer)
{
    if (get_time_ms(timer) > SPAWN_RATE && enemys->num_enemys < enemys->max_enemys) {
        Setup_enemy(enemys, windowSize, renderer, max);
        start_timer(timer);
    }
}