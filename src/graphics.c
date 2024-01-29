#include "graphics.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>


SDL_Window *sdl_window;
SDL_Surface *wall_textures[NUM_WALL_TEXTURES];

char init_graphics() {
	IMG_Init(IMG_INIT_PNG);
	wall_textures[WALL_BRICK] = IMG_Load("assets/wall.png");
	wall_textures[WALL_GREY] = IMG_Load("assets/wall2.png");

	SDL_LogSetAllPriority(SDL_LOG_PRIORITY_DEBUG);

	SDL_Init(SDL_INIT_VIDEO);
	sdl_window = SDL_CreateWindow("ray3d", 0, 0, SDL_WIDTH, SDL_HEIGHT, SDL_WINDOW_SHOWN);
	SDL_SetWindowResizable(sdl_window, SDL_FALSE);

	SDL_UpdateWindowSurface(sdl_window);

	return 1;
}

SDL_Window *graphics_window() {
	return sdl_window;
}

SDL_Surface *graphics_surface() {
	return SDL_GetWindowSurface(sdl_window);
}

void dispose_graphics() {
	SDL_DestroyWindow(sdl_window);
	sdl_window = 0;

	for (unsigned int i = 0; i < NUM_WALL_TEXTURES; i++) {
		SDL_FreeSurface(wall_textures[i]);
	}
}
