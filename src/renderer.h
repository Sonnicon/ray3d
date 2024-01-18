#ifndef H_RENDERER
#define H_RENDERER

#include <SDL2/SDL.h>
#include "world.h"
#include <math.h>


#define RENDERER_RESOLUTION_X 2
#define RENDERER_FOV_X M_PI_2
#define RENDERER_FPS 60

void renderer_update(unsigned int tick);

void renderer_render(SDL_Surface *surface, struct World_Position *source, double angle_vertical);

#endif
