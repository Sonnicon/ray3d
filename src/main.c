#include <SDL2/SDL.h>

#include "graphics.h"
#include "input.h"
#include "player.h"
#include "renderer.h"
#include "world.h"

int main() {
	init_graphics();
	init_input();
	init_player();

	// Temporary world setup
	struct World_Block *block1 = block_create(32, 32, 0, 0, 0, 0, -1, -1, -1, -1);
	player_position(block1, 16., 16., 0., 0.);

	unsigned int tick_last = SDL_GetTicks64();

	while (1) {
		unsigned int tick_now = SDL_GetTicks64();
		unsigned int tick_delta = tick_now - tick_last;
		tick_last = tick_now;
		float delta = (float) tick_delta / 1000.f;

		if (input_poll() & INPUT_RESPONSE_QUIT) {
			break;
		}
		player_update(delta);
		renderer_update(tick_now);
	}

	dispose_graphics();
	return 1;
}
