#include <math.h>

#include "PLAYER.h"
#include "CORE.h"

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
    player.texture = load_texture("Assets/Top_Down_Survivor/rifle/move/survivor-move_rifle_0.png", renderer);
    player.sprite.w = 60;
    player.sprite.h = 60;
    player.sprite.x = dm.w / 2 - player.sprite.w / 2;
    player.sprite.y = dm.h / 2 - player.sprite.h / 2;
    player.center.x = 30;
    player.center.y = 30;
    player.rotation = 0.0;
    player.maxspeed = 5.0;
    player.position.x = 0;
    player.position.y = 0;
    player.velocity.x = 0.0;
    player.velocity.y = 0.0;
    player.acceleration = 0.5;
    return player;
}