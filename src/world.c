#include "world.h"
#include <stdlib.h>
#include <string.h>

#include "math.h"

struct World_Block *block_copy(struct World_Block *from) {
	struct World_Block *new = malloc(sizeof(struct World_Block));
	memcpy(new, from, sizeof(*from));
	return new;
}

#define bp(x) struct World_Block *nearby_block##x

struct World_Block *
block_create(unsigned char w, unsigned char h, bp(0), bp(1), bp(2), bp(3), char face0, char face1, char face2,
			 char face3) {
	struct World_Block *new = malloc(sizeof(struct World_Block));

	new->widths[0] = w;
	new->widths[1] = h;

	new->nearby_blocks[0] = nearby_block0;
	new->nearby_blocks[1] = nearby_block1;
	new->nearby_blocks[2] = nearby_block2;
	new->nearby_blocks[3] = nearby_block3;

	new->face_textures[0] = face0;
	new->face_textures[1] = face1;
	new->face_textures[2] = face2;
	new->face_textures[3] = face3;

	return new;
}

#undef bp


void world_cast_distance(struct World_Position *result, struct World_Position *source, double distance) {
	struct World_Block *iter_block = source->block;

	// Positions we are casting from currently
	// We do this so we can easily alternate between x and y based on direction
	double iter_pos[2] = {source->x, source->y};

	// Angle we are casting at currently	
	double iter_angle = NORMALIZE_ANGLE(source->angle);

	while (distance > 0) {
		// Left and right edges that we might touch
		unsigned char faces[2] = {floor(iter_angle / M_PI_2), 0};
		faces[1] = (faces[0] + 1) % 4;

		// Distances to the edges
		double dist_edge[2] = {DISTANCE_TO_WALL(faces[0], iter_pos, iter_block),
							   DISTANCE_TO_WALL(faces[1], iter_pos, iter_block)};

		// Decide which edge we hit first
		double small_angles[2] = {fmod(iter_angle, M_PI_2), 0.};
		small_angles[1] = M_PI_2 - small_angles[0];
		unsigned char is_right_close = (dist_edge[0] / cos(small_angles[0])) > (dist_edge[1] / cos(small_angles[1]));

		// Find how far we could move
		double length = dist_edge[is_right_close] / cos(small_angles[is_right_close]);

		if (distance >= length) {
			// We shall continue to the next block
			distance -= length;
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
		} else {
			// We can't reach the next block
			iter_pos[0] -= distance * cos(iter_angle);
			iter_pos[1] -= distance * sin(iter_angle);
			break;
		}
	}

	result->x = iter_pos[0];
	result->y = iter_pos[1];
	result->angle = iter_angle - source->angle;
	result->block = iter_block;
}
