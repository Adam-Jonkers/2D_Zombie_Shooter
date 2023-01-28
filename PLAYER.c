#include <math.h>
#include <stdbool.h>

#include "PLAYER.h"
#include "ENEMYS.h"
#include "MAP.h"
#include "CORE.h"

#define PLAYER_MOVE_ANIMATION_LENGTH 19
#define PLAYER_IDLE_ANIMATION_LENGTH 19

#define PLAYER_DAMAGE 2
#define PLAYER_HEALTH 100
#define PLAYER_ACCELERATION 20.0
#define PLAYER_MAX_SPEED 100.0
#define PLAYER_SPEED_MULTIPLIER 2.0

Player_t Setup_player(vec2_t windowSize, SDL_Renderer* renderer)
{
    Player_t player;

    player.alive = true;

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
    player.sprite.x = windowSize.x / 2 - player.sprite.w / 2;
    player.sprite.y = windowSize.y / 2 - player.sprite.h / 2;
    player.center.x = 95/3.5;
    player.center.y = 120/3.5;
    player.rotation = 0.0;
    player.maxSpeed = PLAYER_MAX_SPEED;
    player.position.x = 1000;
    player.position.y = 1000;
    player.camera.x = player.position.x - windowSize.x / 2;
    player.camera.y = player.position.y - windowSize.y / 2;
    player.velocity.x = 0.0;
    player.velocity.y = 0.0;
    player.acceleration = PLAYER_ACCELERATION;
    player.health = PLAYER_HEALTH;
    player.damage = PLAYER_DAMAGE;
    player.sprinting = false;
    player.sprintMultiplier = PLAYER_SPEED_MULTIPLIER;
    
    player.bullets.bullet = NULL;
    player.bullets.num_bullets = 0;

    player.allUpgrades.upgrades = NULL;
    player.allUpgrades.numberOfUpgrades = 0;

    Setup_Upgrades(&player.allUpgrades, renderer);

    player.availableUpgrades = NULL;

    void* ptr = realloc(player.availableUpgrades, 3 * sizeof(Upgrade_t*));
    if (ptr == NULL) {
        printf("Error: Failed to allocate memory for bullet\n");
    } else {
        player.availableUpgrades = ptr;
    }

    printf("Player setup complete\n");

    return player;
}

void Move_player(const Uint8* keyboard_state, Player_t* player, float dt, SDL_Renderer* renderer, Bullets_t* bullets, vec2_t windowSize, vec2_t max, mouse_t mouse, float* noiseMap) 
{
    player->maxSpeed = 100.0f;
    player->acceleration = 20.0f;
    player->velocity = subtract_vec2(player->velocity, divide_vec2(player->velocity, 20.0f));
    player->currentAnimation = &player->idleAnimation;
    player->moveAnimation.speed = 50;
    float speedMultiplier;
    if (keyboard_state[SDL_SCANCODE_LSHIFT]) {
        player->sprinting = true;
    } else {
        player->sprinting = false;
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
    if (mouse.buttons & SDL_BUTTON(SDL_BUTTON_LEFT)) {
        Create_Bullet(renderer, player, bullets);
    }
    speedMultiplier = Get_speed_multiplier(player->position, max, noiseMap);
    if (length_vec2(player->velocity) > (player->maxSpeed * speedMultiplier)) {
        player->velocity = normalize_vec2(player->velocity);
        player->velocity = multiply_vec2(player->velocity, player->maxSpeed * speedMultiplier);
    }

    float move_x = (player->velocity.x) * dt / 1000.0f;
    float move_y = (player->velocity.y) * dt / 1000.0f;

    if (player->sprinting) {
        move_x *= player->sprintMultiplier;
        move_y *= player->sprintMultiplier;
    }
    
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

    player->rotation = get_angle((vec2_t) {player->sprite.x + 30, player->sprite.y + 30}, (vec2_t) {mouse.x, mouse.y});

    if (player->position.x + windowSize.x / 2 > max.x) {
        player->camera.x = max.x - windowSize.x;
    } else if (player->position.x - windowSize.x / 2 < 0) {
        player->camera.x = 0;
    } else {
        player->camera.x = player->position.x - windowSize.x / 2;
    }
    
    if (player->position.y - windowSize.y / 2 < 0) {
        player->camera.y = 0;
    } else if (player->position.y + windowSize.y / 2 > max.y) {
        player->camera.y = max.y - windowSize.y;
    } else {
        player->camera.y = player->position.y - windowSize.y / 2;
    }

    if (player->health <= 0) {
        player->alive = false;
    }
}

void Draw_Player(SDL_Renderer* renderer, Player_t* player, float dt, vec2_t windowSize, vec2_t max)
{
    static int frame = 0;
    static float time = 0;

    time += dt;

    if (player->position.x - windowSize.x / 2 > 0 && player->position.x + windowSize.x < max.x && player->position.y - windowSize.y / 2 > 0 && player->position.y + windowSize.y < max.y) {
        player->sprite.x = windowSize.x / 2 - player->sprite.w / 2;
        player->sprite.y = windowSize.y / 2 - player->sprite.h / 2;
    } 
    if (player->position.x - windowSize.x / 2 < 0 || player->position.x + windowSize.x > max.x) {
        player->sprite.x = player->position.x - player->sprite.w / 2 - player->camera.x;
    } 
    if (player->position.y - windowSize.y / 2 < 0 || player->position.y + windowSize.y > max.y) {
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
    bullets->bullet[bullets->num_bullets]->hitBox = (HitBox_t) {15, 15};
    bullets->bullet[bullets->num_bullets]->damage = player->damage;

    bullets->num_bullets++;
}

void Draw_Bullets(SDL_Renderer* renderer, Player_t* player)
{
    for (int i = 0; i < player->bullets.num_bullets; i++) {
        Draw_Bullet(renderer, player->bullets.bullet[i], player);
    }
}

void Draw_Bullet(SDL_Renderer* renderer, Bullet_t* bullet, Player_t* player) 
{
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

void Remove_Bullet(Bullets_t* bullets, int index)
{
    if (index < 0 || index > bullets->num_bullets) {
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
            printf("Num bullets: %d\n", bullets->num_bullets);
        } else {
            bullets->bullet = ptr;
        }
    }
}

void Remove_Bullets(Bullets_t* bullets)
{
    int all_bullets = bullets->num_bullets;
    for (int i = 0; i < all_bullets; i++) {
        Remove_Bullet(bullets, 0);
    }
    if (bullets->bullet == NULL) {
        printf("bullets destroyed\n");
    } else {
        printf("Error: Failed to destroy bullets %d remaining\n", bullets->num_bullets);
    }
}

void Update_Bullets(Bullets_t* bullets, float dt, Enemys_t* enemys, Player_t* player, SDL_Renderer* renderer)
{
    bool remove = false;
    for (int i = 0; i < bullets->num_bullets; i++) {
        remove = false;
        bullets->bullet[i]->position = add_vec2(bullets->bullet[i]->position, multiply_vec2(bullets->bullet[i]->velocity, dt / 1000.0f));
        bullets->bullet[i]->lifetime += dt;
        if (bullets->bullet[i]->lifetime > bullets->bullet[i]->max_lifetime) {
            remove = true;
        } else {
            for (int j = 0; j < enemys->num_enemys; j++) {
                if (check_collision(subtract_vec2(bullets->bullet[i]->position, player->position), bullets->bullet[i]->hitBox, subtract_vec2(enemys->enemy[j]->position, player->camera), enemys->enemy[j]->hitBox, renderer)) {
                    remove = true;
                    enemys->enemy[j]->health -= bullets->bullet[i]->damage;
                }
            }
        }
        if (remove) {
            Remove_Bullet(bullets, i);
        }
    }
}

/*    UPGRADES    */

void Upgrade_Health(Player_t* player, SDL_Renderer* renderer)
{
    player->health += 20;
    sprintf(player->playerHealthText.text, "Health: %d", (int)player->health);
    load_Text(&player->playerHealthText, renderer);
    printf("\nHealth Upgraded\n");
}

void Upgrade_Damage(Player_t* player, SDL_Renderer* renderer)
{
    player->damage += 0.2;
    printf("\nDamage Upgraded\n");
}

void Upgrade_Speed(Player_t* player, SDL_Renderer* renderer)
{
    player->maxSpeed += 10;
    printf("\nSpeed Upgraded\n");
}

void Upgrade_Knife(Player_t* player, SDL_Renderer* renderer)
{
    player->weapon = KNIFE;
    
    player->moveAnimation.length = PLAYER_MOVE_ANIMATION_LENGTH;
    load_animation(&player->moveAnimation, "Assets/Top_Down_Survivor/knife/move/survivor-move_knife_", renderer);
    player->moveAnimation.speed = 50;

    player->idleAnimation.length = PLAYER_IDLE_ANIMATION_LENGTH;
    load_animation(&player->idleAnimation, "Assets/Top_Down_Survivor/knife/idle/survivor-idle_knife_", renderer);
    player->idleAnimation.speed = 50;
    printf("\nKnife Upgraded\n");
}

void Upgrade_Pistol(Player_t* player, SDL_Renderer* renderer)
{
    player->weapon = PISTOL;
    
    player->moveAnimation.length = PLAYER_MOVE_ANIMATION_LENGTH;
    load_animation(&player->moveAnimation, "Assets/Top_Down_Survivor/handgun/move/survivor-move_handgun_", renderer);
    player->moveAnimation.speed = 50;

    player->idleAnimation.length = PLAYER_IDLE_ANIMATION_LENGTH;
    load_animation(&player->idleAnimation, "Assets/Top_Down_Survivor/handgun/idle/survivor-idle_handgun_", renderer);
    player->idleAnimation.speed = 50;
    printf("\nPistol Upgraded\n");
}

void Upgrade_Rifle(Player_t* player, SDL_Renderer* renderer)
{
    player->weapon = RIFLE;
    
    player->moveAnimation.length = PLAYER_MOVE_ANIMATION_LENGTH;
    load_animation(&player->moveAnimation, "Assets/Top_Down_Survivor/rifle/move/survivor-move_rifle_", renderer);
    player->moveAnimation.speed = 50;

    player->idleAnimation.length = PLAYER_IDLE_ANIMATION_LENGTH;
    load_animation(&player->idleAnimation, "Assets/Top_Down_Survivor/rifle/idle/survivor-idle_rifle_", renderer);
    player->idleAnimation.speed = 50;
    printf("\nRifle Upgraded\n");
}

void Upgrade_Shotgun(Player_t* player, SDL_Renderer* renderer)
{
    player->weapon = SHOTGUN;
    
    player->moveAnimation.length = PLAYER_MOVE_ANIMATION_LENGTH;
    load_animation(&player->moveAnimation, "Assets/Top_Down_Survivor/shotgun/move/survivor-move_shotgun_", renderer);
    player->moveAnimation.speed = 50;

    player->idleAnimation.length = PLAYER_IDLE_ANIMATION_LENGTH;
    load_animation(&player->idleAnimation, "Assets/Top_Down_Survivor/shotgun/idle/survivor-idle_shotgun_", renderer);
    player->idleAnimation.speed = 50;
    printf("\nShotgun Upgraded\n");
}

Upgrade_t* Setup_Upgrade_Health(SDL_Renderer* renderer)
{
    printf("Setup_Upgrade_Health\n");
    void* ptr = calloc(1, sizeof(Upgrade_t));
    Upgrade_t* upgrade = NULL;
    if (ptr == NULL) {
        printf("Error: Failed to allocate memory for Health upgrade\n");
    } else {
        upgrade = ptr;
        printf("Health worked\n");
        upgrade->icon = load_texture("Assets/Upgrades/Health.png", renderer);
        upgrade->rect = (SDL_Rect){0, 0, 0, 0};
        upgrade->upgrade = Upgrade_Health;
    }
    printf("Setup_Upgrade_Health end\n");
    return upgrade;
}

Upgrade_t* Setup_Upgrade_Damage(SDL_Renderer* renderer)
{
    void* ptr = calloc(1, sizeof(Upgrade_t));
    Upgrade_t* upgrade = NULL;
    if (ptr == NULL) {
        printf("Error: Failed to allocate memory for Damage upgrade\n");
    } else {
        upgrade = ptr;
        printf("Damage worked\n");
        upgrade->icon = load_texture("Assets/Upgrades/Damage.png", renderer);
        upgrade->rect = (SDL_Rect){0, 0, 0, 0};
        upgrade->upgrade = Upgrade_Damage;
    }
    return upgrade;
}

Upgrade_t* Setup_Upgrade_Speed(SDL_Renderer* renderer)
{
    void* ptr = calloc(1, sizeof(Upgrade_t));
    Upgrade_t* upgrade = NULL;
    if (ptr == NULL) {
        printf("Error: Failed to allocate memory for Speed upgrade\n");
    } else {
        upgrade = ptr;
        printf("Speed worked\n");
        upgrade->icon = load_texture("Assets/Upgrades/Speed.png", renderer);
        upgrade->rect = (SDL_Rect){0, 0, 0, 0};
        upgrade->upgrade = Upgrade_Speed;
    }
    return upgrade;
}

Upgrade_t* Setup_Upgrade_Knife(SDL_Renderer* renderer)
{
    void* ptr = calloc(1, sizeof(Upgrade_t));
    Upgrade_t* upgrade = NULL;
    if (ptr == NULL) {
        printf("Error: Failed to allocate memory for Knife upgrade\n");
    } else {
    upgrade = ptr;
    upgrade->icon = load_texture("Assets/Upgrades/Knife.png", renderer);
    upgrade->rect = (SDL_Rect){0, 0, 0, 0};
    upgrade->upgrade = Upgrade_Knife;
    }
    return upgrade;
}

Upgrade_t* Setup_Upgrade_Pistol(SDL_Renderer* renderer)
{
    void* ptr = calloc(1, sizeof(Upgrade_t));
    Upgrade_t* upgrade = NULL;
    if (ptr == NULL) {
        printf("Error: Failed to allocate memory for Pistol upgrade\n");
    } else {
        upgrade = ptr;
        upgrade->icon = load_texture("Assets/Upgrades/Pistol.png", renderer);
        upgrade->rect = (SDL_Rect){0, 0, 0, 0};
        upgrade->upgrade = Upgrade_Pistol;
    }
    return upgrade;
}

Upgrade_t* Setup_Upgrade_Rifle(SDL_Renderer* renderer)
{
    void* ptr = calloc(1, sizeof(Upgrade_t));
    Upgrade_t* upgrade = NULL;
    if (ptr == NULL) {
        printf("Error: Failed to allocate memory for rifle upgrade\n");
    } else {
        upgrade = ptr;
        upgrade->icon = load_texture("Assets/Upgrades/Rifle.png", renderer);
        upgrade->rect = (SDL_Rect){0, 0, 0, 0};
        upgrade->upgrade = Upgrade_Rifle;
    }
    return upgrade;
}

Upgrade_t* Setup_Upgrade_Shotgun(SDL_Renderer* renderer)
{
    void* ptr = calloc(1, sizeof(Upgrade_t));
    Upgrade_t* upgrade = NULL;
    if (ptr == NULL) {
        printf("Error: Failed to allocate memory for Shotgun upgrade\n");
    } else {
        upgrade = ptr;
        upgrade->icon = load_texture("Assets/Upgrades/Shotgun.png", renderer);
        upgrade->rect = (SDL_Rect){0, 0, 0, 0};
        upgrade->upgrade = Upgrade_Shotgun;
    }
    return upgrade;
}

void Setup_Upgrades(Upgrades_t* allUpgrades ,SDL_Renderer* renderer)
{
    void* ptr = realloc(allUpgrades->upgrades, 7 * sizeof(Upgrade_t*));
    if (ptr == NULL) {
        printf("Error: Failed to allocate memory for upgrades\n");
    } else {
        allUpgrades->upgrades = ptr;

        allUpgrades->upgrades[0] = Setup_Upgrade_Health(renderer);
        allUpgrades->upgrades[1] = Setup_Upgrade_Damage(renderer);
        allUpgrades->upgrades[2] = Setup_Upgrade_Speed(renderer);
        allUpgrades->upgrades[3] = Setup_Upgrade_Knife(renderer);
        allUpgrades->upgrades[4] = Setup_Upgrade_Pistol(renderer);
        allUpgrades->upgrades[5] = Setup_Upgrade_Rifle(renderer);
        allUpgrades->upgrades[6] = Setup_Upgrade_Shotgun(renderer);

        allUpgrades->numberOfUpgrades = 7;
    }
}

void SelectUpgrades(Upgrade_t* selectedUpgrades[3], Upgrades_t* upgrades, vec2_t windowSize, SDL_Renderer* renderer)
{
    Upgrade_t* upgradePointer = NULL;
    uint8_t upgradeSelected = 0;
    int selectedCount = 0;
    int randomIndex = 0;
    while (selectedCount < 3)
    {
        randomIndex = get_random_number(0,6);
        if ((upgradeSelected & (1 << randomIndex)) == 0) {
            upgradePointer = upgrades->upgrades[randomIndex];
            selectedUpgrades[selectedCount] = upgradePointer;
            selectedUpgrades[selectedCount]->rect = (SDL_Rect){windowSize.x * (selectedCount + 1) / 4 - 100, windowSize.y / 2 - 100, 200, 200};
            upgradeSelected = upgradeSelected | (1 << randomIndex);
            selectedCount++;
        } else {
            printf("Upgrade already selected\n");
        }
    }

}

// create function that checks through upgrades and checks which ones are available
// void Update_Upgrades(Upgrades_t* upgrades, Player_t* player, SDL_Renderer* renderer)
// {

// }