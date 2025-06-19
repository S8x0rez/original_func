#include "motor.h"

#include "ev3api.h"
#include "system.h"
#include "bluetooth.h"

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

}

void all_motor_stop(bool type){
	ev3_motor_stop(PORT_LEFT_MOTOR, type);
	ev3_motor_stop(PORT_RIGHT_MOTOR, type);
	ev3_motor_stop(PORT_ARM_MOTOR, type);
}

void move_task(ROBOT *robot)
{
	
	robot_rotate(robot, robot->route[i][0]);
	robot_forward(robot, robot->route[i][0]);
	
	// usonic start//
	
	robot_rotate(robot, robot->route[i][0]);
	robot_forward(robot, robot->route[i][0]);
	
	// usonic end//
}

void swing_task()
{
	int power = calc_swing_power();

	// ev3_motor_rotate(PORT_ARM_MOTOR, 360, power, false);
}

int calc_swing_power()
{
	int res = 30;

	return res;
}

void move_next_posi()
{

}
