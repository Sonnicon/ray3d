#ifndef H_RENDERER
#define H_RENDERER

#include <math.h>
#include <SDL2/SDL.h>
#include "world.h"

#define RENDERER_RESOLUTION_X 1
#define RENDERER_FOV_X M_PI_2
#define RENDERER_FPS 60

void renderer_update(unsigned int tick);

void renderer_render(SDL_Surface *surface, struct World_Position *source, double angle_vertical);

void smooth_blit(SDL_Surface *src, SDL_Rect *src_rect, SDL_Surface *dst, SDL_Rect *dst_rect);

#endif
