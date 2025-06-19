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

void set_stage(int [][STAGE_BLOCK_COL]);
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
    
    int stage[STAGE_BLOCK_ROW][STAGE_BLOCK_COL];
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

void set_stage(int stage[][STAGE_BLOCK_COL])
{
    for (int i = 0; i < STAGE_BLOCK_ROW; i++) {
        for (int j = 0; j < STAGE_BLOCK_COL; j++) {
            stage[i][j] = (j + i * 2) % 5;
        }
    }
    // 012340123
    // 234012340
    // 401234012
    // 123401234
    // 340123401
    // 012340123
}

void calc_route(ITEMS *obj, int stage[][STAGE_BLOCK_COL])
{
    int straight;

    if (obj->wall->width == 0 && obj->wall->height == 0) {    // wall is none //
        straight = 1;
    }
    else {  // wall is paralle to the stage //
        if (obj->wall->x == 0) {
            if (obj->ball->y < obj->wall->y) straight = 0;
            else straight = 1;
        }
        else if (obj->wall->y == 0) {
            if (obj->ball->x < obj->wall->x) straight = 0;
            else straight = 1;
        }
    }

    // if straight is 0, line ball to arch. else, line ball to hole.   //
}

void print_goal()
{

}
