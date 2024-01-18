#ifndef H_PLAYER
#define H_PLAYER

#include "world.h"
#include "SDL2/SDL.h"

#define PLAYER_SPEED 16

void init_player();

void player_position(struct World_Block *block, double x, double y, double angle, double angle_vertical);

void player_render_view(SDL_Surface *surface);

void player_update(float delta);

#endif
