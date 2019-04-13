#ifndef __UI_H
#define __UI_H
#include "stm32f1xx_hal.h"
#include "game.h"
typedef struct{
    void (*init)(void);
    void (*refresh)(GAME_PAGE *page,
                    step_stack_type *history_step_stack,
                    cursor_type *cursor,
                    WINNER *winner,
                    WHO *who,
                    GAME_MODE *game_mode,
                    PLAY_STATE *playing,
                    START_MENU *start_menu_selection,
                    FINISH_MENU *finish_menu_selection);
}ui_type;
extern ui_type ui;
#endif
