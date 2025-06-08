#ifndef _SYSTEM_H_
#define _SYSTEM_H_

#define MODE_EXIT 0
#define MODE_SELECT 1
#define MODE_MANUAL 2
#define MODE_AUTO 3

int sys_mode_stat;

void cyc_switch_sensor_task()
{
    sys_mode_stat = MODE_SELECT;
}

#endif
