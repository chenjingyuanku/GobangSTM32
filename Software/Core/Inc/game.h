#ifndef __GAME_H
#define __GAME_H
#include "stm32f1xx_hal.h"
/**
 * 开始菜单 
 */
typedef enum 
{
    START_MENU_SELECTION_GAME_START_PVP,
    START_MENU_SELECTION_GAME_START_MVP,
    START_MENU_SELECTION_GAME_START_PVM,
    START_MENU_SELECTION_MAX
}START_MENU;
/** 
 * 结束菜单
 */
typedef enum 
{
    FINISH_MENU_SELECTION_OK,
    FINISH_MENU_SELECTION_RESTART,
    FINISH_MENU_SELECTION_BACK,
    FINISH_MENU_SELECTION_MAX
}FINISH_MENU;
/** 
 * 执棋者
 */
typedef enum 
{
    WHO_WHITE,
    WHO_BLACK
}WHO;
/**
 * 游戏模式
 * PVP 人人模式
 * MVP 机器对人，机器先
 * PVM 人对机器，人先
 */
typedef enum 
{
    PVP,
    MVP,
    PVM
}GAME_MODE;
/** 
 * 游戏运行状态
 * STOPPING 游戏未运行
 * PLAYING  游戏正在运行
 */
typedef enum 
{
    STOPPING,
    PLAYING
}PLAY_STATE;
/** 
 * 棋盘上的点状态
 * CHESSMAN_EMPTY 当前点没有棋子
 * CHESSMAN_BLACK 当前点是黑子
 * CHESSMAN_WHITE 当前点是白子
 */
typedef enum 
{
    CHESSMAN_EMPTY,
    CHESSMAN_BLACK,
    CHESSMAN_WHITE
}CHESSBOARD_POINT;
/** 
 * 赢家
 * WINNER_NONE  没有赢家
 * WINNER_BLACK 黑子是赢家
 * WINNER_WHITE 白子是赢家
 * WINNER_BOTH  和棋
 */
typedef enum 
{
    WINNER_NONE,
    WINNER_BLACK,
    WINNER_WHITE,
    WINNER_BOTH
}WINNER;
/** 
 * 游戏页面
 * GAME_PAGE_START_MENU  开始菜单
 * GAME_PAGE_MAP         棋盘
 * GAME_PAGE_FINISH_MENU 结束菜单
 * GAME_PAGE_MAX         页面数量
 */
typedef enum 
{
    GAME_PAGE_START_MENU,
    GAME_PAGE_MAP,
    GAME_PAGE_FINISH_MENU,
    GAME_PAGE_MAX
}GAME_PAGE;

/** 
 * 历史步保存栈元素结构体
 * x 横坐标 
 * y 纵坐标
 * who 落子者
 */
typedef struct{
    uint8_t x;
    uint8_t y;
    WHO who;
}step_stack_element_type;
/** 
 * 历史步保存栈结构体
 * stack 元素栈
 * length 有效长度
 */
typedef struct{
    step_stack_element_type stack[225];
    uint8_t length;
}step_stack_type;

/** 
 * 光标结构体
 * x 横坐标
 * y 纵坐标
 */
typedef struct{
    uint16_t x;
    uint16_t y;
}cursor_type;


/** 
 * 坐标点得分结构体
 * x 横坐标
 * y 纵坐标
 * score 得分
 */
typedef struct 
{
    int8_t x;
    int8_t y;
    int32_t score;
}score_point_type;



typedef struct{
    //当前界面页
    GAME_PAGE *page;
    //开始菜单选项
    START_MENU *start_menu_selection;
    //结束菜单选项
    FINISH_MENU *finish_menu_selection;
    //历史步保存栈
    step_stack_type *history_step;
    //选择光标
    cursor_type *cursor;
    //游戏模式
    GAME_MODE *game_mode;
    //执棋者
    WHO *who_dropping;
    //获胜者
    WINNER *winner;
    //游戏运行状态
    PLAY_STATE *playing;
    //初始化函数
    void (*init)(void);
    void (*run)(void);
    void (*left_cursor_x_increase)(void);
    void (*left_cursor_x_decrease)(void);
    void (*left_cursor_y_increase)(void);
    void (*left_cursor_y_decrease)(void);
    void (*right_cursor_x_increase)(void);
    void (*right_cursor_x_decrease)(void);
    void (*right_cursor_y_increase)(void);
    void (*right_cursor_y_decrease)(void);
    void (*start_menu_selection_up)(void);
    void (*start_menu_selection_down)(void);
    void (*finish_menu_selection_up)(void);
    void (*finish_menu_selection_down)(void);
    void (*on_left_click)(void);
    void (*on_right_click)(void);
    void (*on_start_menu_click)(void);
    void (*on_finish_menu_click)(void);
}game_type;
extern game_type game;
#endif
