#ifndef _STAGE_ENV_H_
#define _STAGE_ENV_H_

#define STAGE_WIDTH 200
#define STAGE_HEIGHT 200

#define OBSTACLE_WIDTH
#define OBSTACLE_HEIGHT

#define BALL_SIZE
#define GOAL_WIDTH

typedef struct object {
    int x;
    int y;
    
    int width;
    int height;
} OBJECT;

typedef struct robot {
    int x;
    int y;
    int direct;

    int block;
} ROBOT;

void estimate_block();
void estimate_stage();

#endif
