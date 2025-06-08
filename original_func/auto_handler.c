#include "auto_handler.h"

#include "motor.h"
#include "bluetooth.h"
#include "camera.h"
#include "stage_env.h"
#include "system.h"

enum AutoHandleState auto_stat = STAT_NORMAL;

void auto_handler()
{
    const AutoHandlerFunc autoJumpTable[EVENT_MAX][STAT_MAX] = {
        {, },
        {, },
        {, }
    };

    int event;

    while (sys_mode_stat = MODE_AUTO) {
        if (input() == 1) {
            if (event < EVENT_MAX && auto_stat < STAT_MAX) {
                autoJumpTable[event][auto_stat];
            }
        }
    }
}

void calc_swing_power()
{

}

void calc_route()
{

}

void calc_hidden_area()
{

}

void move_next_posi()
{

}

void move_to_ball()
{
    
}
