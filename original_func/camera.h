#ifndef _CAMERA_H_
#define _CAMERA_H_

#define BALL_COLOR
#define GOAL_COLOR

#define GOAL_TARGET 1
#define BALL_TARGET 2

typedef struct image {
    int width;
    int height;
    void *data;
} IMAGE;

void cap_picture();
void calc_target_direction(int);
void calc_target_distance(int);

#endif
