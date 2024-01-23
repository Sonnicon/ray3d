#include "graphics.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

SDL_Window *sdl_window;
SDL_Surface *sdl_surface;
SDL_Surface *wall_texture;

char init_graphics() {
	IMG_Init(IMG_INIT_PNG);
	wall_texture = IMG_Load("assets/wall.png");

	SDL_Init(SDL_INIT_VIDEO);
	sdl_window = SDL_CreateWindow("ray3d", 0, 0, SDL_WIDTH, SDL_HEIGHT, SDL_WINDOW_SHOWN);

	sdl_surface = SDL_GetWindowSurface(sdl_window);

	SDL_UpdateWindowSurface(sdl_window);

	return 1;
}

SDL_Window *graphics_window() {
	return sdl_window;
}

SDL_Surface *graphics_surface() {
	return sdl_surface;
}

void dispose_graphics() {
	sdl_surface = 0;
	SDL_DestroyWindow(sdl_window);
	sdl_window = 0;

	SDL_FreeSurface(wall_texture);
}
