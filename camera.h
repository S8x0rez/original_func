#ifndef _CAMERA_H_
#define _CAMERA_H_

#include "stage_env.h"
#include "camera_task.h"

#define BALL_COLOR_R
#define BALL_COLOR_G
#define BALL_COLOR_B

#define GOAL_COLOR

#define IMAGE_WIDTH 120
#define IMAGE_HEIGHT 160
#define IMAGE_BUF_LEN IMAGE_WIDTH*IMAGE_HEIGHT

#define PI 3.14159
#define TIRE_LEN 5.5 * PI

typedef unsigned char uchar;

extern int g_detected_flag;

typedef struct image {
    int width;
    int height;
    uchar *data;
} IMAGE;

IMAGE *alloc_img(int, int, int);
void free_img(IMAGE *);

void output_bmp_data(IMAGE *, Cam_ResvQueue);

void search_task(ITEMS *);

void cap_picture();

int detect_ball(IMAGE *, ROBOT *, OBJECT *);
int extract_color(uchar *);

int calc_target_direction(int);
int calc_target_distance(double, int, int);

#endif
