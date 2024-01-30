#include "input.h"
#include "SDL2/SDL.h"
#include "player.h"

unsigned char movement_keys = 0;
float movement_mousedelta_x = 0.f;
float movement_mousedelta_y = 0.f;

void init_input() {
	SDL_SetRelativeMouseMode(SDL_TRUE);
}

char input_poll() {
	SDL_Event event;
	while (SDL_PollEvent(&event)) {
		unsigned char is_key_down = 0;
		unsigned char key_value = 0;

		switch (event.type) {
			case (SDL_QUIT):
				return INPUT_RESPONSE_QUIT;
			case (SDL_MOUSEMOTION):
				movement_mousedelta_x += (float) event.motion.xrel / 256.f;
				movement_mousedelta_y += (float) event.motion.yrel / 256.f;
				break;

			case (SDL_MOUSEBUTTONDOWN):
				player_attack(event.button.button);

				break;

			case (SDL_KEYDOWN):
				is_key_down = 1;
			case (SDL_KEYUP):
				switch (event.key.keysym.sym) {
					case (SDLK_UP):
					case (SDLK_j):
						key_value = INPUT_KEY_CAM_UP;
						break;
					case (SDLK_DOWN):
					case (SDLK_k):
						key_value = INPUT_KEY_CAM_DOWN;
						break;
					case (SDLK_LEFT):
					case (SDLK_h):
						key_value = INPUT_KEY_CAM_LEFT;
						break;
					case (SDLK_RIGHT):
					case (SDLK_l):
						key_value = INPUT_KEY_CAM_RIGHT;
						break;
					case (SDLK_w):
						key_value = INPUT_KEY_MOV_UP;
						break;
					case (SDLK_s):
						key_value = INPUT_KEY_MOV_DOWN;
						break;
					case (SDLK_a):
						key_value = INPUT_KEY_MOV_LEFT;
						break;
					case (SDLK_d):
						key_value = INPUT_KEY_MOV_RIGHT;
						break;
				}

				if (is_key_down) movement_keys |= key_value;
				else movement_keys &= ~key_value;
				break;
		}
	}

	return 0;
}

unsigned char input_get_movement() {
	return movement_keys;
}

float input_get_mousedelta_x() {
	return movement_mousedelta_x;
}

void input_reset_mousedelta_x() {
	movement_mousedelta_x = 0.f;
}

float input_get_mousedelta_y() {
	return movement_mousedelta_y;
}

void input_reset_mousedelta_y() {
	movement_mousedelta_y = 0.f;
}
