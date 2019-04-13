#include "ui.h"
#include "ili9341_stm32_driver.h"


static cursor_type previous_cursor = {15,55};
static uint8_t previous_stack_length = 0xff;
static START_MENU previous_start_selection = START_MENU_SELECTION_MAX;
static FINISH_MENU previous_finish_selection = FINISH_MENU_SELECTION_MAX;
void ui_init(void)
{
    lcd.init();
    lcd.fill_screen(YELLOW);
}
void ui_draw_cursor(cursor_type *cursor,WHO *who)
{
    cursor_type point_central_position;
    point_central_position.x = 15+15*cursor->x;
    point_central_position.y = 15*cursor->y+55;
    uint16_t color = WHITE;
    if (*who == WHO_BLACK) {
        color = BLACK;
    }
    
    if( (previous_cursor.x != point_central_position.x ||
        previous_cursor.y != point_central_position.y))
    {
        if(previous_cursor.y < 15*15+55)
        {
            lcd.draw_pixel( previous_cursor.x + 6,
                            previous_cursor.y + 6,
                            OLIVE);
            lcd.draw_pixel( previous_cursor.x + 5,
                            previous_cursor.y + 6,
                            OLIVE);
            lcd.draw_pixel( previous_cursor.x + 4,
                            previous_cursor.y + 6,
                            OLIVE);
            lcd.draw_pixel( previous_cursor.x + 6,
                            previous_cursor.y + 5,
                            OLIVE);
            lcd.draw_pixel( previous_cursor.x + 6,
                            previous_cursor.y + 4,
                            OLIVE);
                            
            lcd.draw_pixel( previous_cursor.x - 6,
                            previous_cursor.y + 6,
                            OLIVE);
            lcd.draw_pixel( previous_cursor.x - 5,
                            previous_cursor.y + 6,
                            OLIVE);
            lcd.draw_pixel( previous_cursor.x - 4,
                            previous_cursor.y + 6,
                            OLIVE);
            lcd.draw_pixel( previous_cursor.x - 6,
                            previous_cursor.y + 5,
                            OLIVE);
            lcd.draw_pixel( previous_cursor.x - 6,
                            previous_cursor.y + 4,
                            OLIVE);
                            
            lcd.draw_pixel( previous_cursor.x + 6,
                            previous_cursor.y - 6,
                            OLIVE);
            lcd.draw_pixel( previous_cursor.x + 5,
                            previous_cursor.y - 6,
                            OLIVE);
            lcd.draw_pixel( previous_cursor.x + 4,
                            previous_cursor.y - 6,
                            OLIVE);
            lcd.draw_pixel( previous_cursor.x + 6,
                            previous_cursor.y - 5,
                            OLIVE);
            lcd.draw_pixel( previous_cursor.x + 6,
                            previous_cursor.y - 4,
                            OLIVE);
                            
            lcd.draw_pixel( previous_cursor.x - 6,
                            previous_cursor.y - 6,
                            OLIVE);
            lcd.draw_pixel( previous_cursor.x - 5,
                            previous_cursor.y - 6,
                            OLIVE);
            lcd.draw_pixel( previous_cursor.x - 4,
                            previous_cursor.y - 6,
                            OLIVE);
            lcd.draw_pixel( previous_cursor.x - 6,
                            previous_cursor.y - 5,
                            OLIVE);
            lcd.draw_pixel( previous_cursor.x - 6,
                            previous_cursor.y - 4,
                            OLIVE);
        }
        else
        {
            lcd.draw_filled_rectangle_coord(9,280,238,318,YELLOW);
            lcd.draw_text("REPENT  MENU",14,285,BLACK,3,YELLOW);
        }
        
    }
    if(point_central_position.y < 15*15+55)
    {
        lcd.draw_pixel( point_central_position.x + 6,
                        point_central_position.y + 6,
                        color);
        lcd.draw_pixel( point_central_position.x + 5,
                        point_central_position.y + 6,
                        color);
        lcd.draw_pixel( point_central_position.x + 4,
                        point_central_position.y + 6,
                        color);
        lcd.draw_pixel( point_central_position.x + 6,
                        point_central_position.y + 5,
                        color);
        lcd.draw_pixel( point_central_position.x + 6,
                        point_central_position.y + 4,
                        color);
                        
        lcd.draw_pixel( point_central_position.x - 6,
                        point_central_position.y + 6,
                        color);
        lcd.draw_pixel( point_central_position.x - 5,
                        point_central_position.y + 6,
                        color);
        lcd.draw_pixel( point_central_position.x - 4,
                        point_central_position.y + 6,
                        color);
        lcd.draw_pixel( point_central_position.x - 6,
                        point_central_position.y + 5,
                        color);
        lcd.draw_pixel( point_central_position.x - 6,
                        point_central_position.y + 4,
                        color);
                        
        lcd.draw_pixel( point_central_position.x + 6,
                        point_central_position.y - 6,
                        color);
        lcd.draw_pixel( point_central_position.x + 5,
                        point_central_position.y - 6,
                        color);
        lcd.draw_pixel( point_central_position.x + 4,
                        point_central_position.y - 6,
                        color);
        lcd.draw_pixel( point_central_position.x + 6,
                        point_central_position.y - 5,
                        color);
        lcd.draw_pixel( point_central_position.x + 6,
                        point_central_position.y - 4,
                        color);
                        
        lcd.draw_pixel( point_central_position.x - 6,
                        point_central_position.y - 6,
                        color);
        lcd.draw_pixel( point_central_position.x - 5,
                        point_central_position.y - 6,
                        color);
        lcd.draw_pixel( point_central_position.x - 4,
                        point_central_position.y - 6,
                        color);
        lcd.draw_pixel( point_central_position.x - 6,
                        point_central_position.y - 5,
                        color);
        lcd.draw_pixel( point_central_position.x - 6,
                        point_central_position.y - 4,
                        color);
    }
    else //绘制按钮选框
    {
        if(cursor->x % 2 == 0)
        {
            lcd.draw_vertical_line(10,288,5,BLACK);
            lcd.draw_vertical_line(122,288,5,BLACK);
            lcd.draw_vertical_line(10,304,5,BLACK);
            lcd.draw_vertical_line(122,304,5,BLACK);
            lcd.draw_horizontal_line(10,288-1,5,BLACK);
            lcd.draw_horizontal_line(122-4,288-1,5,BLACK);
            lcd.draw_horizontal_line(10,304+5,5,BLACK);
            lcd.draw_horizontal_line(122-4,304+5,5,BLACK);
        }
        else
        {
            lcd.draw_vertical_line(154,288,5,BLACK);
            lcd.draw_vertical_line(230,288,5,BLACK);
            lcd.draw_vertical_line(154,304,5,BLACK);
            lcd.draw_vertical_line(230,304,5,BLACK);
            lcd.draw_horizontal_line(154,288-1,5,BLACK);
            lcd.draw_horizontal_line(230-4,288-1,5,BLACK);
            lcd.draw_horizontal_line(154,304+5,5,BLACK);
            lcd.draw_horizontal_line(230-4,304+5,5,BLACK);
        }
        
    }
    
    previous_cursor.x = point_central_position.x;
    previous_cursor.y = point_central_position.y;

}
void ui_draw_chessman(step_stack_element_type step)
{
    lcd.draw_filled_circle(15+15*step.x,
    15*step.y+55,
    5,
    step.who == WHO_BLACK?BLACK:WHITE);
}
void ui_draw_chessboard(step_stack_type *history_step_stack)
{
    //为了效率只有在悔棋和重新开始时才刷新背景
    if(previous_stack_length > history_step_stack->length)
    {
        //绘制标题和棋盘背景
        lcd.draw_rectangle(0,40,240,240,OLIVE);
        //绘制棋盘线
        for(uint8_t i = 0; i < 15; i++)
        {
            lcd.draw_horizontal_line(15,15*i+55,210,BLACK);
            lcd.draw_vertical_line(15+15*i,55,210,BLACK);
        }
        //绘制5个黑点
        lcd.draw_filled_circle(15+15*3,15*3+55,2,BLACK);
        lcd.draw_filled_circle(15+15*11,15*3+55,2,BLACK);
        lcd.draw_filled_circle(15+15*3,15*11+55,2,BLACK);
        lcd.draw_filled_circle(15+15*7,15*7+55,2,BLACK);
        lcd.draw_filled_circle(15+15*11,15*11+55,2,BLACK);
    }
    //为了提高效率，只有在棋子发生变化的时候才刷新棋子
    if(previous_stack_length != history_step_stack->length)
    {
        //绘制棋子
        for(uint8_t step_index = 0; step_index < history_step_stack->length; step_index++)
        {
            ui_draw_chessman(history_step_stack->stack[step_index]);
        }
    }
    previous_stack_length = history_step_stack->length;
}

void ui_draw_start_menu(START_MENU selection)
{
    if(previous_start_selection != selection)
    {
        lcd.draw_text(
            selection==START_MENU_SELECTION_GAME_START_PVP?" >  PVP ":"   PVP ",
            16,90,BLACK,4,YELLOW);
        lcd.draw_text(
            selection==START_MENU_SELECTION_GAME_START_MVP?" >  MVP ":"   MVP ",
            16,130,BLACK,4,YELLOW);
        lcd.draw_text(
            selection==START_MENU_SELECTION_GAME_START_PVM?" >  PVM ":"   PVM ",
            16,170,BLACK,4,YELLOW);
    }
    previous_start_selection = selection;
}
void ui_draw_finish_menu(FINISH_MENU selection,PLAY_STATE playing)
{
    if(previous_finish_selection != selection)
    {
            if(playing == PLAYING)
            {
                lcd.draw_text(
                selection==FINISH_MENU_SELECTION_OK?" >   RESUME <":"    RESUME   ",
                0,120,BLACK,3,YELLOW);
            }
            else
            {
                lcd.draw_text(
                selection==FINISH_MENU_SELECTION_OK?" >   OK     <":"    OK       ",
                0,120,BLACK,3,YELLOW);
            }
        lcd.draw_text(
            selection==FINISH_MENU_SELECTION_RESTART?" >   REPLAY <":"    REPLAY   ",
            0,150,BLACK,3,YELLOW);
        lcd.draw_text(
            selection==FINISH_MENU_SELECTION_BACK?" >   HOME   <":"    HOME     ",
            0,180,BLACK,3,YELLOW);
    }
    previous_finish_selection = selection;
}


void ui_refresh(GAME_PAGE *page,
                step_stack_type *history_step_stack,
                cursor_type *cursor,
                WINNER *winner,
                WHO *who,
                GAME_MODE *game_mode,
                PLAY_STATE *playing,
                START_MENU *start_menu_selection,
                FINISH_MENU *finish_menu_selection)
{
    static GAME_PAGE previous_page = GAME_PAGE_MAX;
    uint8_t reset_flag = 0;
    if(previous_page != *page)
    {
        previous_page = *page;
        reset_flag = 1;
    }
    switch (*page)
    {
        case GAME_PAGE_START_MENU:
        default:
            //开始菜单
            if(reset_flag)
            {
                lcd.fill_screen(YELLOW);
                previous_start_selection = START_MENU_SELECTION_MAX;
            }
            ui_draw_start_menu(*start_menu_selection);
            break;
        case GAME_PAGE_MAP:
            //绘制棋盘
            if(reset_flag)
            {
                previous_cursor.x = 15;
                previous_cursor.y = 55;
                previous_stack_length = 0xff;
                lcd.fill_screen(YELLOW);
                lcd.draw_text("Gobang",64,5,BLACK,3,YELLOW);
                lcd.draw_text("REPENT  MENU",14,285,BLACK,3,YELLOW);
            }
            ui_draw_chessboard(history_step_stack);
            ui_draw_cursor(cursor,who);
            break;
        case GAME_PAGE_FINISH_MENU:
            //结束菜单
            if(reset_flag)
            {
                previous_finish_selection = FINISH_MENU_SELECTION_MAX;
                switch (*winner)
                {
                    case WINNER_BLACK:
                        switch (*game_mode)
                        {
                            case PVP:
                                lcd.draw_text(" BLACK WIN",0,80,BLACK,3,YELLOW);
                                break;
                            case MVP:
                                lcd.draw_text(" YOU LOSE",0,80,BLACK,3,YELLOW);
                                break;
                            case PVM:
                                lcd.draw_text(" YOU WIN ",0,80,BLACK,3,YELLOW);
                                break;
                        }
                        break;
                    case WINNER_WHITE:
                        switch (*game_mode)
                        {
                            case PVP:
                                lcd.draw_text(" WHITE WIN",0,80,BLACK,3,YELLOW);
                                break;
                            case MVP:
                                lcd.draw_text(" YOU WIN ",0,80,BLACK,3,YELLOW);
                                break;
                            case PVM:
                                lcd.draw_text(" YOU LOSE",0,80,BLACK,3,YELLOW);
                                break;
                        }
                        break;
                    case WINNER_BOTH:
                        lcd.draw_text(" DRAW CHESS",0,80,BLACK,3,YELLOW);
                        break;
                
                    default:
                        lcd.draw_text(" WHAT'S UP?",0,80,BLACK,3,YELLOW);
                        break;
                }
            }
            ui_draw_finish_menu(*finish_menu_selection,*playing);
            break;
    }
}

ui_type ui=
{
    (&ui_init),
    (&ui_refresh)
};
