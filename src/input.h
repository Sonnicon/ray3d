#ifndef H_INPUT
#define H_INPUT

#define INPUT_RESPONSE_QUIT 1

#define INPUT_KEY_CAM_UP 1
#define INPUT_KEY_CAM_DOWN 2
#define INPUT_KEY_CAM_LEFT 4
#define INPUT_KEY_CAM_RIGHT 8
#define INPUT_KEY_MOV_UP 16
#define INPUT_KEY_MOV_DOWN 32
#define INPUT_KEY_MOV_LEFT 64
#define INPUT_KEY_MOV_RIGHT 128

void init_input();

char input_poll();

unsigned char input_get_movement();

float input_get_mousedelta_x();

float input_get_mousedelta_y();

void input_reset_mousedelta_x();

void input_reset_mousedelta_y();

#endif
