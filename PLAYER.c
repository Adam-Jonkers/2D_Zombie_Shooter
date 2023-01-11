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

Player_t Setup_player(vec2_t windowsize, SDL_Renderer* renderer)
{
    Player_t player;
    player.moveAnimation.length = PLAYER_MOVE_ANIMATION_LENGTH;
    load_animation(&player.moveAnimation, "Assets/Top_Down_Survivor/rifle/move/survivor-move_rifle_", renderer);
    player.moveAnimation.speed = 50;

    player.idleAnimation.length = PLAYER_IDLE_ANIMATION_LENGTH;
    load_animation(&player.idleAnimation, "Assets/Top_Down_Survivor/rifle/idle/survivor-idle_rifle_", renderer);
    player.idleAnimation.speed = 50;

    player.currentAnimation = &player.idleAnimation;

    int w, h;

    SDL_QueryTexture(player.currentAnimation->animation[0], NULL, NULL, &w, &h);

    player.sprite.w = w/3.5;
    player.sprite.h = h/3.5;
    player.sprite.x = windowsize.x / 2 - player.sprite.w / 2;
    player.sprite.y = windowsize.y / 2 - player.sprite.h / 2;
    player.center.x = 95/3.5;
    player.center.y = 120/3.5;
    player.rotation = 0.0;
    player.maxspeed = 100.0;
    player.position.x = 1000;
    player.position.y = 1000;
    player.camera.x = player.position.x - windowsize.x / 2;
    player.camera.y = player.position.y - windowsize.y / 2;
    player.velocity.x = 0.0;
    player.velocity.y = 0.0;
    player.acceleration = 20.0;

    printf("Player setup complete\n");

    return player;
}

void Move_player(const Uint8* keyboard_state, Player_t* player, float timestep, SDL_Renderer* renderer, Bullets_t* bullets, vec2_t windowsize, vec2_t max) 
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
    if (keyboard_state[SDL_SCANCODE_SPACE]) {
        Create_Bullet(renderer, player, bullets);
    }
    if (length_vec2(player->velocity) > player->maxspeed) {
        player->velocity = normalise_vec2(player->velocity);
        player->velocity = multiply_vec2(player->velocity, player->maxspeed);
    }

    float move_x = (player->velocity.x) * timestep / 1000.0f;
    float move_y = (player->velocity.y) * timestep / 1000.0f;
    
    if (player->position.x + move_x < 0) {
        move_x = 0;
    }
    if (player->position.x + move_x > max.x) {
        move_x = 0;
    }
    if (player->position.y + move_y < 0) {
        move_y = 0;
    }
    if (player->position.y + move_y > max.y) {
        move_y = 0;
    }
    player->position.x += move_x;
    player->position.y += move_y;

    player->rotation = mouse_angle(player->sprite);

    if (player->position.x + windowsize.x / 2 > max.x) {
        player->camera.x = max.x - windowsize.x;
    } else if (player->position.x - windowsize.x / 2 < 0) {
        player->camera.x = 0;
    } else {
        player->camera.x = player->position.x - windowsize.x / 2;
    }
    
    if (player->position.y - windowsize.y / 2 < 0) {
        player->camera.y = 0;
    } else if (player->position.y + windowsize.y / 2 > max.y) {
        player->camera.y = max.y - windowsize.y;
    } else {
        player->camera.y = player->position.y - windowsize.y / 2;
    }
}

void Draw_Player(SDL_Renderer* renderer, Player_t* player, float timestep, vec2_t windowsize, vec2_t max)
{
    static int frame = 0;
    static float time = 0;

    time += timestep;

    if (player->position.x - windowsize.x / 2 > 0 && player->position.x + windowsize.x < max.x && player->position.y - windowsize.y / 2 > 0 && player->position.y + windowsize.y < max.y) {
        player->sprite.x = windowsize.x / 2 - player->sprite.w / 2;
        player->sprite.y = windowsize.y / 2 - player->sprite.h / 2;
    } 
    if (player->position.x - windowsize.x / 2 < 0 || player->position.x + windowsize.x > max.x) {
        player->sprite.x = player->position.x - player->sprite.w / 2 - player->camera.x;
    } 
    if (player->position.y - windowsize.y / 2 < 0 || player->position.y + windowsize.y > max.y) {
        player->sprite.y = player->position.y - player->sprite.h / 2 - player->camera.y;
    }

    SDL_RenderCopyExF(renderer, player->currentAnimation->animation[frame], NULL, &player->sprite, player->rotation * (180 / M_PI), &player->center, SDL_FLIP_NONE);

    if (time > player->currentAnimation->speed) {
        time = 0;
        frame++;
    }
    if (frame > player->currentAnimation->length - 1) {
        frame = 0;
    }
}

void Create_Bullet(SDL_Renderer* renderer, Player_t* player, Bullets_t* bullets)
{
    void* ptr = realloc(bullets->bullet, (bullets->num_bullets + 1) * sizeof(Bullet_t*));
    if (ptr == NULL) {
        printf("Error: Failed to allocate memory for bullet\n");
    } else {
        bullets->bullet = ptr;
    }

    bullets->bullet[bullets->num_bullets] = calloc(1, sizeof(Bullet_t));

    bullets->bullet[bullets->num_bullets]->texture = load_texture("Assets/Bullet/Bullet.png", renderer);
    bullets->bullet[bullets->num_bullets]->position.x = player->position.x + player->sprite.x + player->center.x + 32/3.5 * cos(player->rotation + 1.5708) + 180/3.5 * cos(player->rotation) - 4;
    bullets->bullet[bullets->num_bullets]->position.y = player->position.y + player->sprite.y + player->center.y + 32/3.5 * sin(player->rotation + 1.5708) + 180/3.5 * sin(player->rotation) - 5;
    bullets->bullet[bullets->num_bullets]->velocity =  add_vec2 ((vec2_t) {600 * cos(player->rotation), 600 * sin(player->rotation)}, player->velocity);
    bullets->bullet[bullets->num_bullets]->index = bullets->num_bullets;
    bullets->bullet[bullets->num_bullets]->lifetime = 0;
    bullets->bullet[bullets->num_bullets]->max_lifetime = 5000;

    bullets->num_bullets++;
}

void Draw_Bullets(SDL_Renderer* renderer, Bullets_t* bullets, float timestep, Player_t* player)
{
    for (int i = 0; i < bullets->num_bullets; i++) {
        Draw_Bullet(renderer, bullets->bullet[i], timestep, player);
    }
}

void Draw_Bullet(SDL_Renderer* renderer, Bullet_t* bullet, float timestep, Player_t* player) 
{
    bullet->position = add_vec2(bullet->position, multiply_vec2(bullet->velocity, timestep / 1000.0f));
    SDL_FRect bullet_rect = {bullet->position.x - player->position.x, bullet->position.y - player->position.y, 10, 10};
    SDL_RenderCopyF(renderer, bullet->texture, NULL, &bullet_rect);
}

void Destroy_Bullet(Bullet_t* bullet) 
{
    if (bullet == NULL) {
        printf("Error: Bullet is NULL\n");
        return;
    }

    SDL_DestroyTexture(bullet->texture);
    free(bullet);
    bullet = NULL;
}

void Destroy_Bullets(Bullets_t* bullets)
{
    for (int i = 0; i < bullets->num_bullets; i++) {
        Destroy_Bullet(bullets->bullet[i]);
    }
    free(bullets->bullet);
    bullets->bullet = NULL;
    bullets->num_bullets = 0;
}

void Remove_Bullet(Bullets_t* bullets, int index)
{
    if (index < 0 || index >= bullets->num_bullets) {
        return;
    }

    Destroy_Bullet(bullets->bullet[index]);

    for (int i = index; i < bullets->num_bullets - 1; i++) {
        bullets->bullet[i] = bullets->bullet[i + 1];
        bullets->bullet[i]->index--;
    }

    bullets->num_bullets--;

    if (bullets->num_bullets == 0) {
        free(bullets->bullet);
        bullets->bullet = NULL;
        return;
    } else {
        void* ptr = realloc(bullets->bullet, bullets->num_bullets * sizeof(Bullet_t*));
        if (ptr == NULL) {
            printf("Error: Failed to allocate memory for removing bullet\n");
        } else {
            bullets->bullet = ptr;
        }
    }
}

void Update_Bullets(Bullets_t* bullets, float timestep)
{
    for (int i = 0; i < bullets->num_bullets; i++) {
        bullets->bullet[i]->lifetime += timestep;
        if (bullets->bullet[i]->lifetime > bullets->bullet[i]->max_lifetime) {
            Remove_Bullet(bullets, i);
        }
    }
}
