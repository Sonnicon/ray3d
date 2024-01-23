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
	struct World_Block *block = calloc(1, sizeof(struct World_Block));
	struct World_Block *block2 = calloc(1, sizeof(struct World_Block));
	struct World_Block *block3 = calloc(1, sizeof(struct World_Block));
	struct World_Block *block4 = calloc(1, sizeof(struct World_Block));
	block->widths[0] = 32;
	block->widths[1] = 32;
	block2->widths[0] = 32;
	block2->widths[1] = 32;
	block3->widths[0] = 32;
	block3->widths[1] = 32;
	block4->widths[0] = 32;
	block4->widths[1] = 32;

	block->nearby_blocks[0] = block2;
	block->nearby_blocks_faces[0] = 2;
	block2->nearby_blocks[2] = block;
	block2->nearby_blocks_faces[2] = 0;

	block2->nearby_blocks[0] = block4;
	block2->nearby_blocks_faces[0] = 2;
	block4->nearby_blocks[2] = block2;
	block4->nearby_blocks_faces[2] = 0;

	block2->nearby_blocks[1] = block3;
	block2->nearby_blocks_faces[1] = 3;
	block3->nearby_blocks[3] = block2;
	block3->nearby_blocks_faces[3] = 1;

	block->nearby_blocks[3] = block3;
	block->nearby_blocks_faces[3] = 1;
	block3->nearby_blocks[1] = block;
	block3->nearby_blocks_faces[1] = 3;

	block4->nearby_blocks[0] = block4;
	block4->nearby_blocks_faces[0] = 2;

	player_position(block, 16., 16., 0., 0.);

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
