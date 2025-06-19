#ifndef _MOTOR_H_
#define _MOTOR_H_

#include "stage_env.h"
#include "stdbool.h"

void swing_task();

void move_task();
void robot_rotate(ROBOT *, int);

void hit_ball(char *);
void all_motor_stop(bool);

#endif
