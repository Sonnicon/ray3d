#ifndef H_GRAPHICS
#define H_GRAPHICS

#include <SDL2/SDL.h>

#define SDL_WIDTH 640
#define SDL_HEIGHT 480

#define NUM_WALL_TEXTURES 2

enum Wall_Texture {
	WALL_BRICK = 0,
	WALL_GREY
};

char init_graphics();

void dispose_graphics();

SDL_Surface *graphics_surface();

SDL_Window *graphics_window();

#endif
