#include "manual_handler.h"

#include "motor.h"
#include "bluetooth.h"
#include "stage_env.h"
#include "system.h"

enum ManualHandleState manual_stat = STAT_NORMAL;

void manual_handler()
{
    const ManualHandlerFunc manualJumpTable[EVENT_MAX][STAT_MAX] = {
        {, , },
        {, , },
        {, , }
    };

    int event;

    while (sys_mode_stat == MODE_MANUAL) {
        if (input() == 1) {
            if (event < EVENT_MAX && manual_stat < STAT_MAX) {
                manualJumpTable[event][manual_stat];
            }
        }
    }
}
