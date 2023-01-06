#include <math.h>

#include "PLAYER.h"
#include "CORE.h"

#define PLAYER_MOVING 0
#define PLAYER_IDLE 1

#define PLAYER_MOVE_ANIMATION_LENGTH 19
#define PLAYER_IDLE_ANIMATION_LENGTH 19

double mouse_angle(SDL_FRect sprite)
{
    int mouse_x, mouse_y;
    SDL_GetMouseState(&mouse_x, &mouse_y);
    double angle = atan2(mouse_y - sprite.y - 30, mouse_x - sprite.x - 30);
    return angle;
}

Player_t Setup_player(SDL_DisplayMode dm, SDL_Renderer* renderer)
{
    Player_t player;
    player.moveAnimation.length = PLAYER_MOVE_ANIMATION_LENGTH;
    load_animation(&player.moveAnimation, "Assets/Top_Down_Survivor/rifle/move/survivor-move_rifle_", renderer);
    player.idleAnimation.length = PLAYER_IDLE_ANIMATION_LENGTH;
    load_animation(&player.idleAnimation, "Assets/Top_Down_Survivor/rifle/idle/survivor-idle_rifle_", renderer);
    player.sprite.w = 60;
    player.sprite.h = 60;
    player.sprite.x = dm.w / 2 - player.sprite.w / 2;
    player.sprite.y = dm.h / 2 - player.sprite.h / 2;
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
    if (keyboard_state[SDL_SCANCODE_LSHIFT]) {
        player->maxspeed = 200.0f;
        player->acceleration = 50.0f;
    }
    if (keyboard_state[SDL_SCANCODE_W]) {
        player->velocity.x += player->acceleration * cos(player->rotation);
        player->velocity.y += player->acceleration * sin(player->rotation);
    }
    if (keyboard_state[SDL_SCANCODE_S]) {
        player->velocity.x += -player->acceleration * cos(player->rotation);
        player->velocity.y += -player->acceleration * sin(player->rotation);
    }
    if (keyboard_state[SDL_SCANCODE_A]) {
        player->velocity.x += -player->acceleration * cos(player->rotation + 1.5708);
        player->velocity.y += -player->acceleration * sin(player->rotation + 1.5708);
    }
    if (keyboard_state[SDL_SCANCODE_D]) {
        player->velocity.x += player->acceleration * cos(player->rotation + 1.5708);
        player->velocity.y += player->acceleration * sin(player->rotation + 1.5708);
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

    if (length_vec2(player->velocity) > 10.0f) {
        player->action = PLAYER_MOVING;
    } else {
        player->action = PLAYER_IDLE;
    }
}

void Draw_Player(SDL_Renderer* renderer, Player_t* player)
{
    static int frame = 0;
    static int currentAnimation = PLAYER_IDLE;
    if (frame == 0 ) {
        currentAnimation = player->action;
    }
    if (currentAnimation == PLAYER_MOVING) {
        SDL_RenderCopyExF(renderer, player->moveAnimation.animation[frame], NULL, &player->sprite, player->rotation * (180 / M_PI), &player->center, SDL_FLIP_NONE);
    } else if (currentAnimation == PLAYER_IDLE) {
        SDL_RenderCopyExF(renderer, player->idleAnimation.animation[frame], NULL, &player->sprite, player->rotation * (180 / M_PI), &player->center, SDL_FLIP_NONE);
    }
    frame++;
    if (frame > 18)
    {
        frame = 0;
    }
}