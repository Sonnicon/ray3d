#include "renderer.h"
#include <SDL2/SDL_surface.h>
#include "graphics.h"
#include "math.h"
#include "player.h"
#include "world.h"

#define PROJ_A ((double)(SDL_WIDTH / (2 * tan(RENDERER_FOV_X * 0.5))))

unsigned int tick_last_frame = 1;
unsigned int frame_count = 0;

extern SDL_Surface *wall_texture;

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


	SDL_Rect draw_area;
	draw_area.w = RENDERER_RESOLUTION_X;

	SDL_Rect tex_area;
	tex_area.w = 1;
	tex_area.y = 0;
	tex_area.h = 32;

	// x-beams
	for (unsigned short column = 0; column <= SDL_WIDTH; column += RENDERER_RESOLUTION_X) {
		draw_area.x = column;

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

		// Distance to the edges
		double dist_edge[2];

		unsigned char is_right_close;
		unsigned char faces[2];

		while (distance < RENDERER_MAXDIST) {
			// Left and right edges that we might touch
			faces[0] = floor(iter_angle / M_PI_2);
			faces[1] = (faces[0] + 1) % 4;

			// Distances to the edges
			dist_edge[0] = DISTANCE_TO_WALL(faces[0], iter_pos, iter_block);
			dist_edge[1] = DISTANCE_TO_WALL(faces[1], iter_pos, iter_block);

			// Decide which edge we hit first
			double small_angles[2] = {fmod(iter_angle, M_PI_2), 0.};
			small_angles[1] = M_PI_2 - small_angles[0];
			is_right_close =
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

		float constant = SDL_HEIGHT * 8.0 / (distance * fabs(cos(column_angle - source->angle)));
		float variable = SDL_HEIGHT * (tan(angle_vertical) + 0.5);

		int draw_top = (int) floorf(variable - constant);
		int draw_bottom = (int) ceilf(variable + constant);

		// Walls
		if (distance <= RENDERER_MAXDIST) {
			tex_area.x = (int) iter_pos[faces[is_right_close] % 2 == 0];

			draw_area.y = draw_top;
			draw_area.h = draw_bottom - draw_top;

			smooth_blit(wall_texture, &tex_area, surface, &draw_area);
		}
		// Ceiling
		draw_area.y = 0;
		draw_area.h = draw_top;
		SDL_FillRect(surface, &draw_area, 0x044AA);
		// Floor
		draw_area.y = draw_bottom >= 1 ? draw_bottom - 1 : 0;
		draw_area.h = SDL_HEIGHT;
		SDL_FillRect(surface, &draw_area, 0x444444);

	}

	SDL_UpdateWindowSurface(graphics_window());
}

void smooth_blit(SDL_Surface *src, SDL_Rect *src_rect, SDL_Surface *dst, SDL_Rect *dst_rect) {
	float pixel_height = (float) dst_rect->h / (float) src_rect->h;

	int draw_top = dst_rect->y;
	int draw_bottom = dst_rect->y + dst_rect->h;

	unsigned short skip_top = 0, skip_bottom = 0;

	// Trimming top
	if (draw_top < 0.) {
		int trim_top = -draw_top;
		float diff = (float) trim_top / pixel_height;
		skip_top = (unsigned short) ceilf(diff);
		draw_top = (int) roundf(((float) skip_top - diff) * pixel_height);
	}

	// Trimming Bottom
	if (draw_bottom > SDL_HEIGHT) {
		unsigned short trim_bottom = dst_rect->y + dst_rect->h - SDL_HEIGHT;
		float diff = (float) trim_bottom / pixel_height;
		skip_bottom = (unsigned short) ceilf(diff);
		draw_bottom = SDL_HEIGHT - roundf(((float) skip_bottom - diff) * pixel_height);
	}

	// Main segment
	SDL_Rect take_rect;
	take_rect.x = src_rect->x;
	take_rect.w = src_rect->w;
	take_rect.y = src_rect->y + skip_top;
	take_rect.h = src_rect->h - skip_top - skip_bottom;

	SDL_Rect draw_rect;
	draw_rect.x = dst_rect->x;
	draw_rect.w = dst_rect->w;
	draw_rect.y = draw_top;
	draw_rect.h = draw_bottom - draw_top;

	SDL_LowerBlitScaled(src, &take_rect, dst, &draw_rect);

	// Top square
	if (skip_top) {
		draw_rect.y = 0;
		draw_rect.h = draw_top;

		take_rect.y = src_rect->y + skip_top - 1;
		take_rect.h = 1;

		SDL_BlitScaled(src, &take_rect, dst, &draw_rect);
	}

	// Bottom square
	if (skip_bottom) {
		draw_rect.y = draw_bottom;
		draw_rect.h = SDL_HEIGHT - draw_bottom;

		take_rect.y = src_rect->y + src_rect->h - skip_bottom;
		take_rect.h = 1;

		SDL_BlitScaled(src, &take_rect, dst, &draw_rect);
	}
}
