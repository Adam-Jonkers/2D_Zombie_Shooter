#ifndef RPG_GAME_H
#define RPG_GAME_H

typedef struct {

} Global_t;

typedef struct {

} Game_t;

Global_t Setup_Global(void);

void Cleanup_Global(Global_t* global);

Game_t Setup_Game(void);

void Run_Game(Game_t* game);

void close_Game(Game_t* game);

#endif