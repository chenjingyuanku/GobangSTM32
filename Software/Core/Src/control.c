#include "control.h"
#include "multi_button.h"
#include "gpio.h"
#include "main.h"
#include "adc.h"
#include "tim.h"
#include "game.h"

uint16_t joy_values[4];

button button_left;
button button_right;

button joy_left_x_small;
button joy_left_y_small;
button joy_left_x_large;
button joy_left_y_large;
button joy_right_x_small;
button joy_right_y_small;
button joy_right_x_large;
button joy_right_y_large;

uint8_t read_button_left(void)
{
    return (uint8_t)HAL_GPIO_ReadPin(BUTTON_LEFT_GPIO_Port, BUTTON_LEFT_Pin);
}
uint8_t read_button_right(void)
{
    return (uint8_t)HAL_GPIO_ReadPin(BUTTON_RIGHT_GPIO_Port, BUTTON_RIGHT_Pin);
}
uint8_t read_left_joy_x(void)
{
    if(joy_values[2] > 3800)
    {
        return 2;//右
    }
    if(joy_values[2] < 200)
    {
        return 1;//左
    }
    return 0;//中间
}
uint8_t read_left_joy_y(void)
{
    if(joy_values[3] > 3800)
    {
        return 2;//上
    }
    if(joy_values[3] < 200)
    {
        return 1;//下
    }
    return 0;//中间
}
uint8_t read_right_joy_x(void)
{
    if(joy_values[0] > 3800)
    {
        return 2;//右
    }
    if(joy_values[0] < 200)
    {
        return 1;//左
    }
    return 0;//中间
}
uint8_t read_right_joy_y(void)
{
    if(joy_values[1] > 3800)
    {
        return 2;//上
    }
    if(joy_values[1] < 200)
    {
        return 1;//下
    }
    return 0;//中间
}













//*****************************************************************
//*****************************************************************
//*************************************按键事件*********************
//*****************************************************************
//*****************************************************************

/**
 * on_button_left_click
 * @description:
 *   左侧按钮单击事件
 * @param: {void *} btn 区分当前按下的按钮
 * @return: {类型}
 */
void on_button_left_click(void *btn)
{
    switch (*game.page)
    {
        case GAME_PAGE_START_MENU:
            game.on_start_menu_click();
            break;
        case GAME_PAGE_MAP:
            game.on_left_click();
            break;
        case GAME_PAGE_FINISH_MENU:
            game.on_finish_menu_click();
            break;
        default:
            break;
    }
}
/**
 * on_button_right_click
 * @description:
 *   右侧按钮单击事件
 * @param: {void *} btn 区分当前按下的按钮
 * @return: {类型}
 */
void on_button_right_click(void *btn)
{

    switch (*game.page)
    {
        case GAME_PAGE_START_MENU:
            game.on_start_menu_click();
            break;
        case GAME_PAGE_MAP:
            game.on_right_click();
            break;
        case GAME_PAGE_FINISH_MENU:
            game.on_finish_menu_click();
            break;
        default:
            break;
    }
}
//*****************************************************************
//*****************************************************************
//*************************************摇杆事件*********************
//*****************************************************************
//*****************************************************************

/**
 * on_joy_left_x_small
 * @description:
 *   左侧摇杆左移事件
 * @param: {void *} btn 区分当前按下的按钮
 * @return: {类型}
 */
void on_joy_left_x_small(void *btn)
{
    switch (*game.page)
    {
        case GAME_PAGE_MAP:
            game.left_cursor_x_decrease();
            break;
        case GAME_PAGE_FINISH_MENU:
            /* code */
            break;
        default:
            break;
    }
}
/**
 * on_joy_left_x_large
 * @description:
 *   左侧摇杆右移事件
 * @param: {void *} btn 区分当前按下的按钮
 * @return: {类型}
 */
void on_joy_left_x_large(void *btn)
{
    switch (*game.page)
    {
        case GAME_PAGE_MAP:
            game.left_cursor_x_increase();
            break;
        case GAME_PAGE_FINISH_MENU:
            /* code */
            break;
        default:
            break;
    }
}
/**
 * on_joy_left_y_small
 * @description:
 *   左侧摇杆下移事件
 * @param: {void *} btn 区分当前按下的按钮
 * @return: {类型}
 */
void on_joy_left_y_small(void *btn)
{
    switch (*game.page)
    {
        case GAME_PAGE_START_MENU:
            game.start_menu_selection_down();
            break;
        case GAME_PAGE_MAP:
            game.left_cursor_y_decrease();
            break;
        case GAME_PAGE_FINISH_MENU:
            game.finish_menu_selection_down();
            break;
        default:
            break;
    }
}
/**
 * on_joy_left_y_large
 * @description:
 *   左侧摇杆上移事件
 * @param: {void *} btn 区分当前按下的按钮
 * @return: {类型}
 */
void on_joy_left_y_large(void *btn)
{
    switch (*game.page)
    {
        case GAME_PAGE_START_MENU:
            game.start_menu_selection_up();
            break;
        case GAME_PAGE_MAP:
            game.left_cursor_y_increase();
            break;
        case GAME_PAGE_FINISH_MENU:
            game.finish_menu_selection_up();
            break;
        default:
            break;
    }
}
/**
 * on_joy_right_x_small
 * @description:
 *   右侧摇杆左移事件
 * @param: {void *} btn 区分当前按下的按钮
 * @return: {类型}
 */
void on_joy_right_x_small(void *btn)
{
    switch (*game.page)
    {
        case GAME_PAGE_MAP:
            game.right_cursor_x_decrease();
            break;
        case GAME_PAGE_FINISH_MENU:
            /* code */
            break;
        default:
            break;
    }
}
/**
 * on_joy_right_x_large
 * @description:
 *   右侧摇杆右移事件
 * @param: {void *} btn 区分当前按下的按钮
 * @return: {类型}
 */
void on_joy_right_x_large(void *btn)
{
    switch (*game.page)
    {
        case GAME_PAGE_MAP:
            game.right_cursor_x_increase();
            break;
        case GAME_PAGE_FINISH_MENU:
            /* code */
            break;
        default:
            break;
    }
}
/**
 * on_joy_right_y_small
 * @description:
 *   右侧摇杆下移事件
 * @param: {void *} btn 区分当前按下的按钮
 * @return: {类型}
 */
void on_joy_right_y_small(void *btn)
{
    switch (*game.page)
    {
        case GAME_PAGE_START_MENU:
            game.start_menu_selection_down();
            break;
        case GAME_PAGE_MAP:
            game.right_cursor_y_decrease();
            break;
        case GAME_PAGE_FINISH_MENU:
            game.finish_menu_selection_down();
            break;
        default:
            break;
    }
}
/**
 * on_joy_right_y_large
 * @description:
 *   右侧摇杆上移事件
 * @param: {void *} btn 区分当前按下的按钮
 * @return: {类型}
 */
void on_joy_right_y_large(void *btn)
{
    switch (*game.page)
    {
        case GAME_PAGE_START_MENU:
            game.start_menu_selection_up();
            break;
        case GAME_PAGE_MAP:
            game.right_cursor_y_increase();
            break;
        case GAME_PAGE_FINISH_MENU:
            game.finish_menu_selection_up();
            break;
        default:
            break;
    }
}


void control_init(void)
{
    HAL_ADC_Start_DMA(&hadc1,(uint32_t *)joy_values,4);
    HAL_Delay(100);

    button_init(&button_left,&read_button_left,0);
    button_init(&button_right,&read_button_right,0);

    button_init(&joy_left_x_small,&read_left_joy_x,1);
    button_init(&joy_left_y_small,&read_left_joy_y,1);
    button_init(&joy_left_x_large,&read_left_joy_x,2);
    button_init(&joy_left_y_large,&read_left_joy_y,2);
    button_init(&joy_right_x_small,&read_right_joy_x,1);
    button_init(&joy_right_y_small,&read_right_joy_y,1);
    button_init(&joy_right_x_large,&read_right_joy_x,2);
    button_init(&joy_right_y_large,&read_right_joy_y,2);

    button_attach(&button_left,SINGLE_CLICK,&on_button_left_click);
    button_attach(&button_right,SINGLE_CLICK,&on_button_right_click);
    
    button_attach(&joy_left_x_small,SINGLE_CLICK,&on_joy_left_x_small);
    button_attach(&joy_left_y_small,SINGLE_CLICK,&on_joy_left_y_small);
    button_attach(&joy_left_x_large,SINGLE_CLICK,&on_joy_left_x_large);
    button_attach(&joy_left_y_large,SINGLE_CLICK,&on_joy_left_y_large);
    button_attach(&joy_right_x_small,SINGLE_CLICK,&on_joy_right_x_small);
    button_attach(&joy_right_y_small,SINGLE_CLICK,&on_joy_right_y_small);
    button_attach(&joy_right_x_large,SINGLE_CLICK,&on_joy_right_x_large);
    button_attach(&joy_right_y_large,SINGLE_CLICK,&on_joy_right_y_large);

    
    button_attach(&joy_left_x_small,LONG_PRESS_HOLD,&on_joy_left_x_small);
    button_attach(&joy_left_y_small,LONG_PRESS_HOLD,&on_joy_left_y_small);
    button_attach(&joy_left_x_large,LONG_PRESS_HOLD,&on_joy_left_x_large);
    button_attach(&joy_left_y_large,LONG_PRESS_HOLD,&on_joy_left_y_large);
    button_attach(&joy_right_x_small,LONG_PRESS_HOLD,&on_joy_right_x_small);
    button_attach(&joy_right_y_small,LONG_PRESS_HOLD,&on_joy_right_y_small);
    button_attach(&joy_right_x_large,LONG_PRESS_HOLD,&on_joy_right_x_large);
    button_attach(&joy_right_y_large,LONG_PRESS_HOLD,&on_joy_right_y_large);


    button_start(&button_left);
    button_start(&button_right);
    button_start(&joy_left_x_small);
    button_start(&joy_left_y_small);
    button_start(&joy_left_x_large);
    button_start(&joy_left_y_large);
    button_start(&joy_right_x_small);
    button_start(&joy_right_y_small);
    button_start(&joy_right_x_large);
    button_start(&joy_right_y_large);

    HAL_TIM_Base_Start_IT(&htim4);
}
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
    if(htim == &htim4)
    {
        button_ticks();
    }
}
control_type control=
{
    (&control_init)
};