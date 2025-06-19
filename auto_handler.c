#include "auto_handler.h"

#include "motor.h"
#include "bluetooth.h"
#include "camera.h"
#include "stage_env.h"
#include "system.h"

#include "app.h"


enum AutoHandleState {
    STAT_SEARCH,
    STAT_AUTO_MOVE,
    STAT_SWING,
    STAT_GOAL
};

enum SearchEvent {
    EVENT_CAPTURE,
    EVENT_MOVE,
    EVENT_CALC
};

void calc_route(ITEMS *, int [][STAGE_BLOCK_COL]);
void print_goal();

enum AutoHandleState g_auto_stat;

void auto_handler()
{
    ROBOT robot = {15, 20, -90};
    OBJECT ball = {15, 30, 4, 4};
    OBJECT hole = {170, 170, 8, 8};
#ifdef NORMAL_STAGE
    OBJECT wall;
#else
    OBJECT wall = {0, 120, 120, 0};
#endif

    ITEMS obj = {&robot, &ball, &hole, &wall};
    
    int stage[STAGE_BLOCK_ROW][STAGE_BLOCK_COL] = {};
    int route[2][2];
    
    ev3_lcd_draw_string("Auto mode runnnig", 0, l_font_property.height * 3);
    bt_connect();
    
	act_tsk(CAM_TASK);
    tslp_tsk(1000);

	while (!ev3_button_is_pressed(ENTER_BUTTON)) {
            ev3_lcd_draw_string("READY? (push entry button)", 0, l_font_property.height * 4);
    }

	ev3_motor_rotate(PORT_ARM_MOTOR, -360, 100, true);
    g_auto_stat = STAT_SEARCH;
    
    while (g_sys_mode_stat == MODE_AUTO) {
        if (g_auto_stat == STAT_SWING) {
            ev3_lcd_draw_string("SWING", 0, l_font_property.height * 4);
            swing_task();
            
            if (1) {    // swing direction  //
                robot_rotate(90);
            }
            else {
                robot_rotate(-90);
            }
            g_auto_stat = STAT_SEARCH;
        }
        else if (g_auto_stat == STAT_SEARCH) {
            robot_rotate(&robot, 45);
            search_task(&obj);
            calc_route(&obj, stage);
            g_auto_stat = STAT_AUTO_MOVE;
        }
        else if (g_auto_stat == STAT_AUTO_MOVE) {
            move_task(&robot);

            if (g_detected_flag == 1) {
                g_auto_stat = STAT_SWING;
            }
            else {
                tslp_tsk(500);
                g_auto_stat = STAT_SEARCH;
            }
        }
        else {
            print_goal();
        }
    }
}

void calc_route(ITEMS *obj, int stage[][STAGE_BLOCK_COL])
{
    if (obj->wall->width == 0 && obj->wall->height == 0) {    // wall is none //
        obj->robot->route[0][0] = sqrt(pow(obj->hole->x - obj->robot->x, 2) + pow(obj->hole->y - obj->robot->y, 2));
        obj->robot->route[0][1] = 0;

        obj->robot->route[1][0] = 0;
        obj->robot->route[1][1] = 0;
    }
}

void print_goal()
{

}
