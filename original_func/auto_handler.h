#ifndef _AUTO_HANDLER_H_
#define _AUTO_HANDLER_H_

enum AutoHandleState {
    STAT_NORMAL,
    STAT_SEARCH,
    STAT_MOVE,
    STAT_MAX
};

enum AutoHandleEvent {
    EVENT_NONE,
    EVENT_TOUCH_PRESSED,
    EVENT_MAX
};

typedef void (*AutoHandlerFunc)();

void auto_handler();

void calc_swing_power();
void calc_route();
void calc_hidden_area();
void move_next_posi();
void move_to_ball();

#endif
