#ifndef H_GRAPHICS
#define H_GRAPHICS

#include <SDL2/SDL.h>

#define SDL_WIDTH 640
#define SDL_HEIGHT 480

char init_graphics();

void dispose_graphics();

SDL_Surface *graphics_surface();

SDL_Window *graphics_window();

#endif
