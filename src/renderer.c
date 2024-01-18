#include <SDL2/SDL.h>
#include "world.h"
#include "graphics.h"
#include "renderer.h"
#include "math.h"
#include "player.h"

#define PROJ_A ((double)(SDL_WIDTH / (2 * tan(RENDERER_FOV_X * 0.5))))

unsigned int tick_last_frame = 1;
unsigned int frame_count = 0;

void renderer_update(unsigned int tick) {
	if (tick - tick_last_frame < 1000 / RENDERER_FPS) {
		return;
	}
	frame_count++;
	tick_last_frame = tick;

	player_render_view(graphics_surface());
}

void renderer_render(SDL_Surface *surface, struct World_Position *source, double angle_vertical) {
	SDL_FillRect(surface, 0, 0);
	// x-beams
	for (unsigned short column = 0; column <= SDL_WIDTH; column += RENDERER_RESOLUTION_X) {
		struct World_Block *iter_block = source->block;

		// Positions we are casting from currently
		// We do this so we can easily alternate between x and y based on direction
		double iter_pos[2] = {source->x, source->y};
		// Angle we are casting at currently
		//const float column_angle = NORMALIZE_ANGLE(source_angle + COLUMN_ANGLE(column));
		const double column_angle = NORMALIZE_ANGLE(source->angle + atan((column - SDL_WIDTH * 0.5) / PROJ_A));

		double iter_angle = column_angle;

		// How far we have casted so far
		double distance = 0.;

		while (distance < 512.) {
			// Left and right edges that we might touch
			unsigned char faces[2] = {floor(iter_angle / M_PI_2), 0};
			faces[1] = (faces[0] + 1) % 4;

			// Distances to the edges
			double dist_edge[2] = {DISTANCE_TO_WALL(faces[0], iter_pos, iter_block),
								   DISTANCE_TO_WALL(faces[1], iter_pos, iter_block)};

			// Decide which edge we hit first
			double small_angles[2] = {fmod(iter_angle, M_PI_2), 0.};
			small_angles[1] = M_PI_2 - small_angles[0];
			unsigned char is_right_close =
					(dist_edge[0] / cos(small_angles[0])) > (dist_edge[1] / cos(small_angles[1]));

			// Move to the edge we can
			distance += dist_edge[is_right_close] / cos(small_angles[is_right_close]);

			// Move as far as we can along the other edge
			iter_pos[faces[is_right_close ^ 1] % 2] += dist_edge[is_right_close] / tan(small_angles[!is_right_close]) *
													   ((faces[is_right_close ^ 1] >= 2) * 2 - 1);

			struct World_Block *next_block = iter_block->nearby_blocks[faces[is_right_close]];
			unsigned char next_face = iter_block->nearby_blocks_faces[faces[is_right_close]];

			// No next block
			if (!next_block) break;
			iter_block = next_block;

			// Coordinate transformations	
			unsigned char ff = faces[is_right_close];
			unsigned char nf_0_3 = next_face % 3 == 0;
			unsigned char ff_0_3 = ff % 3 == 0;
			unsigned char nf_0_2 = next_face % 2 == 0;
			double nf_wh = next_block->widths[!nf_0_2];
			double nf_xy = iter_pos[ff % 2 == 0];
			iter_pos[nf_0_2] = (!nf_0_3 ^ !ff_0_3) * nf_xy + !(!nf_0_3 ^ !ff_0_3) * (nf_wh - nf_xy);
			iter_pos[!nf_0_2] = (next_face >= 2) * next_block->widths[next_face - 2];

			// Angle transformation
			iter_angle = NORMALIZE_ANGLE(iter_angle + M_PI_2 * (2 - ff + next_face));
		}

		if (distance > 500.) continue;
		double straight_distance = distance * fabs(cos(column_angle - source->angle));
		double draw_height = 16 * (double) SDL_HEIGHT;// - straight_distance;// * sin(column_angle);

		double v_offset = tan(angle_vertical) * straight_distance * (double) SDL_HEIGHT;


		double theta1 = atan((draw_height * -0.5 + v_offset) / straight_distance);
		double theta2 = atan((draw_height * 0.5 + v_offset) / straight_distance);

		double bottom = ((double) SDL_HEIGHT / 2) + tan(theta1);
		double top = ((double) SDL_HEIGHT / 2) + tan(theta2);

		unsigned short draw_bottom = (unsigned short) bottom;
		unsigned short draw_top = (unsigned short) top;

		if (bottom < 0.) {
			draw_bottom = 0;
		} else if (bottom > SDL_HEIGHT) {
			continue;
		}
		if (top > SDL_HEIGHT) {
			draw_top = SDL_HEIGHT;
		} else if (top < 0.) {
			continue;
		}

		SDL_Rect draw_area;
		draw_area.x = column;
		draw_area.w = RENDERER_RESOLUTION_X;
		draw_area.y = draw_bottom;
		draw_area.h = draw_top - draw_bottom;

		straight_distance *= 1.5;
		straight_distance = straight_distance > 255 ? 255 : straight_distance;
		SDL_FillRect(surface, &draw_area, 0xFF - ((unsigned char) straight_distance));
	}

	SDL_UpdateWindowSurface(graphics_window());
}
