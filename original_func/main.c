#include "auto_handler.h"
#include "manual_handler.h"
#include "system.h"

void select_mode_stat();
void print_exit();

int main(void)
{
    sys_mode_stat = MODE_SELECT;
    select_mode_stat();

    while (sys_mode_stat) {
        if (sys_mode_stat == MODE_AUTO) {
            auto_handler();
        }
        else if (sys_mode_stat == MODE_MANUAL) {
            manual_handler();
        }
    }

    print_exit();
}

void select_mode_stat()
{
    // LCDからモードを選択  //
    // sys_mode_stat = モード番号   //
}

void print_exit()
{
    // 終了状態をLCDに表示  //
}
