#include "motor.h"

#include "ev3api.h"
#include "system.h"
#include "bluetooth.h"

#include <math.h>

#define PI 3.141592

int calc_swing_power();

void hit_ball(char *chr)
{
	int power;
	int flag = 1;

	while (flag == 1) {
		if (*chr == 'h') {
			read_port(chr);
		}
		else {
			flag = 0;
		}
	}

	power = (int)(*chr);
	ev3_motor_rotate(PORT_ARM_MOTOR, 360, power, true);
}

void robot_rotate(ROBOT *robot, int angle)
{
	ev3_gyro_sensor_reset(PORT_GYRO_SENSOR);
	int rotate_angle = 0;

	if (angle < 0){
		while (rotate_angle > angle) {
			ev3_motor_steer(PORT_LEFT_MOTOR, PORT_RIGHT_MOTOR, 20, -100);
			rotate_angle = ev3_gyro_sensor_get_angle(PORT_GYRO_SENSOR);
		}
	}
	else if (angle > 0) {
		while (rotate_angle < angle) {
			ev3_motor_steer(PORT_LEFT_MOTOR, PORT_RIGHT_MOTOR, 20, 100);
			rotate_angle = ev3_gyro_sensor_get_angle(PORT_GYRO_SENSOR);
		}
	}

	all_motor_stop(true);
	angle = ev3_gyro_sensor_get_angle(PORT_GYRO_SENSOR);
	robot->direct = robot->direct + angle;
}

void robot_forward(ROBOT *robot, int dist)
{
	double tire_size = 5.5;
	int res = 360 * dist / tire_size;

	ev3_motor_rotate(PORT_LEFT_MOTOR, res, 50, false);
	ev3_motor_rotate(PORT_RIGHT_MOTOR, res, 50, true);

	robot->x = dist * cos(PI * dist / 180);
	robot->y = dist * sin(PI * dist / 180);
}

void all_motor_stop(bool type)
{
	ev3_motor_stop(PORT_LEFT_MOTOR, type);
	ev3_motor_stop(PORT_RIGHT_MOTOR, type);
	ev3_motor_stop(PORT_ARM_MOTOR, type);
}

void move_task(ROBOT *robot)
{
	
	robot_rotate(robot, robot->route[0][0]);
	robot_forward(robot, robot->route[0][1]);
	
	// usonic start//
	
	robot_rotate(robot, robot->route[1][0]);
	robot_forward(robot, robot->route[1][1]);
	
	// usonic end//
}

void swing_task(ROBOT *robot, OBJECT *ball, int x, int y)
{
	int power = 100 * sqrt(pow(x - ball->x, 2) + pow(y - ball->y, 2)) / (180 * 1.4142);
	int sign = 1;	// 1(+) or -1(-)	//
	int arm_angle;

	ev3_motor_rotate(PORT_ARM_MOTOR, 360 * sign, power, true);
	arm_angle = ev3_motor_get_counts(PORT_ARM_MOTOR) % 360;
    ev3_motor_rotate(PORT_ARM_MOTOR, arm_angle % 360, 20, true);
}
