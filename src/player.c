#include "player.h"
#include "input.h"
#include "renderer.h"
#include "world.h"

struct World_Position *player_pos;
double player_angle_vertical;

void init_player() {
	player_pos = malloc(sizeof(struct World_Position));
	player_pos->x = 0.f;
	player_pos->y = 0.f;
	player_pos->angle = 0.f;
	player_pos->block = 0;
	player_angle_vertical = 0.f;
}

void player_position(struct World_Block *block, double x, double y, double angle, double angle_vertical) {
	player_pos->x = x;
	player_pos->y = y;
	player_pos->angle = angle;
	player_pos->block = block;
	player_angle_vertical = angle_vertical;
}

void player_render_view(SDL_Surface *surface) {
	renderer_render(surface, player_pos, player_angle_vertical);
}

void player_update(float delta) {
	unsigned char movement_keys = input_get_movement();

	// Mouse turning x
	float rotate_angle = input_get_mousedelta_x();
	input_reset_mousedelta_x();

	// Keyboard turning x
	if (movement_keys & INPUT_KEY_CAM_LEFT) {
		rotate_angle -= delta * 2;
	} else if (movement_keys & INPUT_KEY_CAM_RIGHT) {
		rotate_angle += delta * 2;
	}
	if (rotate_angle) {
		player_pos->angle = NORMALIZE_ANGLE(player_pos->angle + rotate_angle);
	}

	// Mouse turning y
	rotate_angle = -input_get_mousedelta_y() * 0.6f;
	input_reset_mousedelta_y();

	// Keyboard turning y
	if (movement_keys & INPUT_KEY_CAM_UP) {
		rotate_angle += delta;
	} else if (movement_keys & INPUT_KEY_CAM_DOWN) {
		rotate_angle -= delta;
	}
	if (fabsf(rotate_angle) > 0.001f) {
		if (rotate_angle < 0.f && player_angle_vertical > M_PI && player_angle_vertical < M_PI * 1.55f) {
			player_angle_vertical = 1.55f * M_PI;
		} else if (rotate_angle > 0.f && player_angle_vertical < M_PI && player_angle_vertical > M_PI * 0.45f) {
			player_angle_vertical = M_PI * 0.45f;
		} else {
			player_angle_vertical = NORMALIZE_ANGLE(player_angle_vertical + rotate_angle);
		}
	}

	// Keyboard movement
	double distance[2] = {0, 0};
	if (movement_keys & INPUT_KEY_MOV_UP) {
		distance[1] -= PLAYER_SPEED * delta;
	}
	if (movement_keys & INPUT_KEY_MOV_DOWN) {
		distance[1] += PLAYER_SPEED * delta;
	}
	if (movement_keys & INPUT_KEY_MOV_LEFT) {
		distance[0] += PLAYER_SPEED * delta;
	}
	if (movement_keys & INPUT_KEY_MOV_RIGHT) {
		distance[0] -= PLAYER_SPEED * delta;
	}

	if (fabs(distance[0]) + fabs(distance[1]) < 0.001) {
		return;
	}

	double move_distance[2];
	move_distance[0] = distance[0] * cos(player_pos->angle + M_PI_2) + distance[1] * cos(player_pos->angle);
	move_distance[1] = distance[1] * sin(player_pos->angle) + distance[0] * sin(player_pos->angle + M_PI_2);

	double move_length = sqrt(move_distance[0] * move_distance[0] + move_distance[1] * move_distance[1]);

	struct World_Position move_source;
	move_source.x = player_pos->x;
	move_source.y = player_pos->y;
	move_source.angle = atan(move_distance[1] / move_distance[0]) + (M_PI * (move_distance[0] >= 0));
	move_source.block = player_pos->block;

	struct World_Position move_result;
	world_cast_distance(&move_result, &move_source, move_length);
	player_pos->x = move_result.x;
	player_pos->y = move_result.y;
	player_pos->angle += move_result.angle;
	player_pos->block = move_result.block;
}
