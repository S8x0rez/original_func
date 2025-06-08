#ifndef _MANUAL_HANDLER_H_
#define _MANUAL_HANDLER_H_

enum ManualHandleState {
    STAT_NORMAL,
    STAT_MOVE,
    STAT_HIT,
    STAT_MAX
};

enum ManualHandleEvent {
    EVENT_MOVE,
    EVENT_HIT,
    EVENT_TOUCH_PRESSED,
    EVENT_MAX
};

typedef void (*ManualHandlerFunc)();

void manual_handler();

void judge_control();

#endif
