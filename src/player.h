#ifndef H_PLAYER
#define H_PLAYER

#include "SDL2/SDL.h"
#include "world.h"

#define PLAYER_SPEED 32

void init_player();

void player_position(struct World_Block *block, double x, double y, double angle, double angle_vertical);

void player_render_view(SDL_Surface *surface);

void player_update(float delta);

#endif
