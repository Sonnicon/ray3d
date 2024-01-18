#include "graphics.h"

SDL_Window *sdl_window;
SDL_Surface *sdl_surface;

char init_graphics() {
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
}
