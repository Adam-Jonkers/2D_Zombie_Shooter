#include <math.h>

#include "PLAYER.h"
#include "CORE.h"

#define PLAYER_MOVE_ANIMATION_LENGTH 19
#define PLAYER_IDLE_ANIMATION_LENGTH 19

double mouse_angle(SDL_FRect sprite)
{
    int mouse_x, mouse_y;
    SDL_GetMouseState(&mouse_x, &mouse_y);
    double angle = atan2(mouse_y - sprite.y - 30, mouse_x - sprite.x - 30);
    return angle;
}

Player_t Setup_player(int window_x, int window_y, SDL_Renderer* renderer)
{
    Player_t player;
    player.moveAnimation.length = PLAYER_MOVE_ANIMATION_LENGTH;
    load_animation(&player.moveAnimation, "Assets/Top_Down_Survivor/rifle/move/survivor-move_rifle_", renderer);
    player.moveAnimation.speed = 50;

    player.idleAnimation.length = PLAYER_IDLE_ANIMATION_LENGTH;
    load_animation(&player.idleAnimation, "Assets/Top_Down_Survivor/rifle/idle/survivor-idle_rifle_", renderer);
    player.idleAnimation.speed = 50;

    player.currentAnimation = &player.idleAnimation;

    player.sprite.w = 60;
    player.sprite.h = 60;
    player.sprite.x = window_x / 2 - player.sprite.w / 2;
    player.sprite.y = window_y / 2 - player.sprite.h / 2;
    player.center.x = 30;
    player.center.y = 30;
    player.rotation = 0.0;
    player.maxspeed = 100.0;
    player.position.x = 0;
    player.position.y = 0;
    player.velocity.x = 0.0;
    player.velocity.y = 0.0;
    player.acceleration = 20.0;
    return player;
}

void Move_player(const Uint8* keyboard_state, Player_t* player, float timestep) 
{
    player->maxspeed = 100.0f;
    player->acceleration = 20.0f;
    player->velocity = subtract_vec2(player->velocity, divide_vec2(player->velocity, 20.0f));
    player->currentAnimation = &player->idleAnimation;
    player->moveAnimation.speed = 50;
    if (keyboard_state[SDL_SCANCODE_LSHIFT]) {
        player->maxspeed = 200.0f;
        player->acceleration = 50.0f;
        player->moveAnimation.speed = 30;
    }
    if (keyboard_state[SDL_SCANCODE_W]) {
        player->velocity.x += player->acceleration * cos(player->rotation);
        player->velocity.y += player->acceleration * sin(player->rotation);
        player->currentAnimation = &player->moveAnimation;
    }
    if (keyboard_state[SDL_SCANCODE_S]) {
        player->velocity.x += -player->acceleration * cos(player->rotation);
        player->velocity.y += -player->acceleration * sin(player->rotation);
        player->currentAnimation = &player->moveAnimation;
    }
    if (keyboard_state[SDL_SCANCODE_A]) {
        player->velocity.x += -player->acceleration * cos(player->rotation + 1.5708);
        player->velocity.y += -player->acceleration * sin(player->rotation + 1.5708);
        player->currentAnimation = &player->moveAnimation;
    }
    if (keyboard_state[SDL_SCANCODE_D]) {
        player->velocity.x += player->acceleration * cos(player->rotation + 1.5708);
        player->velocity.y += player->acceleration * sin(player->rotation + 1.5708);
        player->currentAnimation = &player->moveAnimation;
    }
    if (keyboard_state[SDL_SCANCODE_LCTRL]) {
        player->velocity = subtract_vec2(player->velocity, divide_vec2(player->velocity, 4.0f));
    }
    if (length_vec2(player->velocity) > player->maxspeed) {
        player->velocity = normalise_vec2(player->velocity);
        player->velocity = multiply_vec2(player->velocity, player->maxspeed);
    }

    player->position.x += (player->velocity.x) * timestep / 1000.0f;
    player->position.y += (player->velocity.y) * timestep / 1000.0f;

    player->rotation = mouse_angle(player->sprite);
}

void Draw_Player(SDL_Renderer* renderer, Player_t* player, float timestep)
{
    static int frame = 0;
    static float time = 0;

    time += timestep;

    SDL_RenderCopyExF(renderer, player->currentAnimation->animation[frame], NULL, &player->sprite, player->rotation * (180 / M_PI), &player->center, SDL_FLIP_NONE);

    if (time > player->currentAnimation->speed) {
        time = 0;
        frame++;
    }
    if (frame > player->currentAnimation->length - 1)
    {
        frame = 0;
    }
}

Bullet_t* Create_Bullet(SDL_Renderer* renderer, Player_t* player)
{
    Bullet_t* bullet = malloc(sizeof(Bullet_t));
    bullet->texture = load_texture("Assets/Bullet/Bullet.png", renderer);
    bullet->position = player->position;
    bullet->velocity = (vec2_t) {600 * cos(player->rotation), 600 * sin(player->rotation)};
    return bullet;
}

void Draw_Bullets(SDL_Renderer* renderer, Bullet_t** bullets, int bullet_count, float timestep)
{
    for (int i = 0; i < bullet_count; i++) {
        Draw_Bullet(renderer, bullets[i], timestep);
    }
}

void Draw_Bullet(SDL_Renderer* renderer, Bullet_t* bullet, float timestep) 
{
    bullet->position = add_vec2(bullet->position, multiply_vec2(bullet->velocity, timestep / 1000.0f));
    SDL_FRect bullet_rect = {bullet->position.x, bullet->position.y, 10, 10};
    SDL_RenderCopyF(renderer, bullet->texture, NULL, &bullet_rect);
}

void Destroy_Bullet(Bullet_t* bullet) 
{
    SDL_DestroyTexture(bullet->texture);
    free(bullet);
}