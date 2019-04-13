#include "game.h"
#include "string.h"
#include "main.h"
#include "math.h"
/**
 * 历史步保存栈的操作的返回状态
 * STACK_EXECUTE_OK 执行成功
 * STACK_IS_FULL    栈已满
 * STACK_IS_EMPTY   栈为空
 */
typedef enum
{
  STACK_EXECUTE_OK,
  STACK_IS_FULL,
  STACK_IS_EMPTY
} STEP_STACK_FUNCTION_EXECUTE_STATE;
//历史步保存栈
step_stack_type history_step;
/**
 * push_step_stack
 * @description:
 *   将当前步入栈
 * @param: {uint8_t} x 横坐标
 * @param: {uint8_t} y 纵坐标
 * @param: {WHO} who 谁下的子
 * @return: {STEP_STACK_FUNCTION_EXECUTE_STATE} 当前操作的完成状态
 */
STEP_STACK_FUNCTION_EXECUTE_STATE push_step_stack(uint8_t x, uint8_t y, WHO who)
{
  if (history_step.length > 224)
    return STACK_IS_FULL;
  history_step.stack[history_step.length].x = x;
  history_step.stack[history_step.length].y = y;
  history_step.stack[history_step.length].who = who;
  history_step.length++;
  return STACK_EXECUTE_OK;
}
/**
 * pop_step_stack
 * @description:
 *   将栈顶的元素出栈
 * @param: {step_stack_element_type *} step 接收变量的指针,为NULL只出栈不接收
 * @return: {STEP_STACK_FUNCTION_EXECUTE_STATE} 当前操作的完成状态
 */
STEP_STACK_FUNCTION_EXECUTE_STATE pop_step_stack(step_stack_element_type *step)
{
  if (history_step.length == 0)
    return STACK_IS_EMPTY;
  history_step.length--;
  if (step != NULL)
  {
    step->x = history_step.stack[history_step.length].x;
    step->y = history_step.stack[history_step.length].y;
    step->who = history_step.stack[history_step.length].who;
  }
  return STACK_EXECUTE_OK;
}

//开始菜单选项
START_MENU start_menu_selection = START_MENU_SELECTION_GAME_START_PVP;
//结束菜单选项
FINISH_MENU finish_menu_selection = FINISH_MENU_SELECTION_OK;
//游戏模式
GAME_MODE game_mode = PVP;
//游戏运行状态
PLAY_STATE playing = STOPPING;
//执棋者
WHO who_dropping = WHO_BLACK;
//游戏页面
GAME_PAGE page = GAME_PAGE_START_MENU;
//悔棋按钮选中标志
uint8_t repent_button_selected = 0;
//棋子矩阵
CHESSBOARD_POINT chessman_matrix[15][15];
//选择光标
cursor_type cursor = {7, 7};
//获胜者
WINNER winner = WINNER_NONE;

/**
 * game_init
 * @description:
 *   游戏初始化
 * @param: {void}
 * @return: {void}
 */
void game_init(void)
{
  //清空棋子矩阵
  memset(chessman_matrix, CHESSMAN_EMPTY, sizeof(chessman_matrix));
  //清空栈
  history_step.length = 0;
  //开始菜单选项
  start_menu_selection = START_MENU_SELECTION_GAME_START_PVP;
  //结束菜单选项
  finish_menu_selection = FINISH_MENU_SELECTION_OK;
  //游戏模式
  game_mode = PVP;
  //游戏运行状态
  playing = STOPPING;
  //执棋者
  who_dropping = WHO_BLACK;
  //游戏页面
  page = GAME_PAGE_START_MENU;
  //悔棋按钮选中标志
  repent_button_selected = 0;
  //选择光标
  cursor.x = cursor.y = 7;
  //获胜者
  winner = WINNER_NONE;
}
/**
 * game_who_is_winner
 * @description:
 *   判断获胜者
 * @param: {void}
 * @return: {WINNER} 返回当前获胜者
 */
WINNER game_who_is_winner(void)
{
  for (uint8_t i = 0; i < 15; i++)
  {
    for (uint8_t j = 0; j < 15; j++)
    {
      //循环遍历每个非空点
      if (chessman_matrix[i][j] != CHESSMAN_EMPTY)
      {
        if (i >= 2 && i < 15 - 2 && j >= 2 && j < 15 - 2)
        {
          if (
              (chessman_matrix[i][j] == chessman_matrix[i - 2][j - 2] &&
               chessman_matrix[i][j] == chessman_matrix[i - 1][j - 1] &&
               chessman_matrix[i][j] == chessman_matrix[i + 2][j + 2] &&
               chessman_matrix[i][j] == chessman_matrix[i + 1][j + 1]) ||
              (chessman_matrix[i][j] == chessman_matrix[i + 1][j - 1] &&
               chessman_matrix[i][j] == chessman_matrix[i + 2][j - 2] &&
               chessman_matrix[i][j] == chessman_matrix[i - 1][j + 1] &&
               chessman_matrix[i][j] == chessman_matrix[i - 2][j + 2]))
          {
            return chessman_matrix[i][j] == CHESSMAN_BLACK ? WINNER_BLACK : WINNER_WHITE;
          }
        }
        if (i < 15 - 4)
        {
          if (chessman_matrix[i][j] == chessman_matrix[i + 1][j] &&
              chessman_matrix[i][j] == chessman_matrix[i + 2][j] &&
              chessman_matrix[i][j] == chessman_matrix[i + 3][j] &&
              chessman_matrix[i][j] == chessman_matrix[i + 4][j])
          {
            return chessman_matrix[i][j] == CHESSMAN_BLACK ? WINNER_BLACK : WINNER_WHITE;
          }
        }
        if (j < 15 - 4)
        {
          if (chessman_matrix[i][j] == chessman_matrix[i][j + 1] &&
              chessman_matrix[i][j] == chessman_matrix[i][j + 2] &&
              chessman_matrix[i][j] == chessman_matrix[i][j + 3] &&
              chessman_matrix[i][j] == chessman_matrix[i][j + 4])
          {
            return chessman_matrix[i][j] == CHESSMAN_BLACK ? WINNER_BLACK : WINNER_WHITE;
          }
        }
      }
    }
  }
  //如果历史步数达到棋盘总大小且没有五连子，说明和棋
  if (history_step.length == 15 * 15)
  {
    return WINNER_BOTH;
  }
  return WINNER_NONE;
}
/**
 * game_drop_chessman
 * @description:
 *   下一个棋子
 * @param: {uint8_t} x 落子点横坐标
 * @param: {uint8_t} y 落子点纵坐标
 * @return: {void}
 */
void game_drop_chessman(uint8_t x, uint8_t y)
{
  STEP_STACK_FUNCTION_EXECUTE_STATE res;
  //将当前步入栈
  res = push_step_stack(x, y, who_dropping);
  if (res != STACK_EXECUTE_OK)
  {
    Error_Handler();
  }
  //修改矩阵中当前点棋子状态
  chessman_matrix[x][y] =
      who_dropping == WHO_BLACK ? CHESSMAN_BLACK : CHESSMAN_WHITE;
  //切换下棋者
  who_dropping = who_dropping == WHO_BLACK ? WHO_WHITE : WHO_BLACK;
}
/**
 * game_repent
 * @description:
 *   悔棋
 * @param: {uint8_t} repent_times 悔棋个数
 * @return: {void}
 */
void game_repent(uint8_t repent_times)
{
  step_stack_element_type step;
  STEP_STACK_FUNCTION_EXECUTE_STATE res;
  if (history_step.length < repent_times)
    return;
  while (repent_times--)
  {
    //将保存的历史步出栈
    res = pop_step_stack(&step);
    if (res != STACK_EXECUTE_OK)
    {
      Error_Handler();
    }
    //将矩阵当前位置清空
    chessman_matrix[step.x][step.y] = CHESSMAN_EMPTY;
    //切换下棋者
    who_dropping = who_dropping == WHO_BLACK ? WHO_WHITE : WHO_BLACK;
  }
}
/**
 * game_on_start_menu_click
 * @description:
 *   开始菜单点击事件
 * @param: {void}
 * @return: {void}
 */
void game_on_start_menu_click(void)
{
  //切换到游戏地图界面
  page = GAME_PAGE_MAP;
  //修改为对应的游戏模式，一个菜单对应一种模式
  game_mode = (GAME_MODE)start_menu_selection;
  //切换游戏状态到运行中
  playing = PLAYING;
}
/**
 * game_on_finish_menu_click
 * @description:
 *   结束菜单点击事件
 * @param: {void}
 * @return: {void}
 */
void game_on_finish_menu_click(void)
{
  //暂时保存上一局游戏模式
  GAME_MODE previous_mode = game_mode;
  switch (finish_menu_selection)
  {
  //点击确定，什么也不操作，回到棋盘界面，玩家可以复盘
  case FINISH_MENU_SELECTION_OK:
    page = GAME_PAGE_MAP;
    break;
  //重新开始游戏
  case FINISH_MENU_SELECTION_RESTART:
    //初始化游戏
    game_init();
    //切换到棋盘界面
    page = GAME_PAGE_MAP;
    //模式还原成上一局的模式
    game_mode = previous_mode;
    //游戏状态切换回游戏中
    playing = PLAYING;
    break;
  //返回到首页，选择新模式
  case FINISH_MENU_SELECTION_BACK:
    //初始化游戏
    game_init();
    //切换到启动页
    page = GAME_PAGE_START_MENU;
    break;

  default:
    break;
  }
}
/**
 * game_on_left_click
 * @description:
 *   左键按下事件
 * @param: {void}
 * @return: {void}
 */
void game_on_left_click(void)
{
  //当前不是黑方落子
  if (who_dropping != WHO_BLACK && playing == PLAYING)
  {
    return;
  }
  //点击菜单，纵坐标超出棋盘范围说明当前光标在按钮上
  if (cursor.y > 14)
  {
    //左侧按钮被点击，当横坐标为偶数，选择的是左侧悔棋按钮
    if (cursor.x % 2 == 0)
    {
      //悔棋要连悔2步
      game_repent(2);
    }
    else //右侧按钮被点击，横坐标为奇数，选择的是右侧菜单按钮
    {
      //切换到菜单页面
      page = GAME_PAGE_FINISH_MENU;
    }
    return;
  }
  //当前位置有子，不操作
  if (chessman_matrix[cursor.x][cursor.y] != CHESSMAN_EMPTY)
  {
    return;
  }
  //当前是电脑回合，不操作
  if (game_mode == MVP)
  {
    return;
  }
  //一般情况，下一个棋子
  game_drop_chessman(cursor.x, cursor.y);
}
void game_on_right_click(void)
{
  //当前不是白方落子
  if (who_dropping != WHO_WHITE && playing == PLAYING)
  {
    return;
  }
  //点击菜单
  if (cursor.y > 14)
  {
    if (cursor.x % 2 == 0)
    {
      game_repent(2);
    }
    else
    {
      page = GAME_PAGE_FINISH_MENU;
    }
    return;
  }
  //当前位置有子
  if (chessman_matrix[cursor.x][cursor.y] != CHESSMAN_EMPTY)
  {
    return;
  }
  //当前是电脑回合
  if (game_mode == PVM)
  {
    return;
  }
  game_drop_chessman(cursor.x, cursor.y);
}

/** 
 * 方向枚举
 * LT  左上
 * T   上
 * RT  右上
 * L   左
 * R   右
 * LB  左下
 * B   下
 * RB  右下
 */
typedef enum
{
  LT = 0,
  T,
  RT,
  L,
  R,
  LB,
  B,
  RB
} dir_type;

/**
 * 扫描向量结构体
 * 保存相关扫描参数，用于分数计算
 */
typedef struct
{
  //区间内我方棋子总个数，遇到连续2空子停止计数 = 0
  uint8_t my_count;
  //区间内我方连续棋子个数 = 0
  uint8_t my_consecutive_count;
  //一个方向的扫描距离，最大四格，到敌方棋子、棋盘边缘截止 = 0
  uint8_t my_search_area;
  //区间内敌方棋子总个数，遇到连续2空子停止计数 = 0
  uint8_t enemy_count;
  //区间内敌方连续棋子个数 = 0
  uint8_t enemy_consecutive_count;
  //一个方向的扫描距离，最大四格，到我方棋子、棋盘边缘截止 = 0
  uint8_t enemy_search_area;
  //我方棋子连续标志 = 1
  uint8_t my_consecutive_flag;
  //敌方棋子连续标志 = 1
  uint8_t enemy_consecutive_flag;
  //我方继续扫描标志 = 1
  uint8_t my_scan_flag;
  //敌方继续扫描标志 = 1
  uint8_t enemy_scan_flag;
  //连续空子计数标志 = 0
  uint8_t consecutive_empty_count_flag;
  //上一个扫描到的点 = CHESSMAN_EMPTY
  CHESSBOARD_POINT previous_point;
  //我方颜色
  WHO me;
  //对方颜色
  WHO enemy;
  //当前扫描方向
  dir_type dir;
  //当前点横坐标
  uint8_t x;
  //当前点纵坐标
  uint8_t y;
} vector_type;

/**
   * game_calc_my_score
   * @description:
   *   计算己方得分
   * @param: {vector_type} dir_a 方向A
   * @param: {vector_type} dir_b 方向B，在A的对面
   * @return: {number} 返回计算出来的分值
   */
int32_t game_calc_my_score(vector_type dir_a, vector_type dir_b)
{
  //两头堵死的个数
  uint8_t close_sides = 0;
  //无害的区域范围
  uint8_t valid_area = dir_a.my_search_area + dir_b.my_search_area;
  //无害区域范围内，我方棋子总个数
  int32_t my_count_sum = dir_a.my_count + dir_b.my_count;
  //有效区域不足5，不能形成五连子
  if (valid_area < 4)
  {
    return 5; //当前方向被封死了，返回一个很小的值
  }
  //如果有一边连续棋子个数等于扫描区间，封闭边缘+1，为了调整后面打分时候的权重
  if (dir_a.my_search_area == dir_a.my_consecutive_count)
  {
    close_sides++;
  }
  if (dir_b.my_search_area == dir_b.my_consecutive_count)
  {
    close_sides++;
  }
  switch (dir_a.my_consecutive_count + dir_b.my_consecutive_count)
  {
  case 0:
    //以下是当前方向上分散棋子个数，间隔不超过1
    switch (my_count_sum)
    {
    case 0:
      return 0;
    case 1:
      return 100;
    case 2:
      return 200;
    case 3:
      return 400;
    default:
      return 2000;
    }
  case 1:
    //2连，2头空
    if (close_sides == 0)
    {
      //活跳三
      if (my_count_sum == 2)
      {
        return 4000;
      }
      //冲四
      if (my_count_sum >= 3)
      {
        return 5000;
      }
      //普通连2
      return 1000;
    } //眠二
    else
    {
      return 100;
    }
  case 2:
    //3连，2头空
    if (close_sides == 0)
    {
      //冲四
      if (my_count_sum >= 3)
      {
        return 5000;
      }
      //普通连3
      return 4000;
    } //眠三
    else
    {
      return 1000;
    }
  case 3:
    //活四
    if (close_sides == 0)
    {
      return 30000;
    } //冲四
    else
    {
      return 5000;
    }
  default:
    //五连子了，返回最大得分
    return 50 * 10000;
  }
}

/**
   * game_calc_enemy_score
   * @description:
   *   计算敌方得分
   * @param: {vector_type} dir_a 方向A
   * @param: {vector_type} dir_b 方向B，在A的对面
   * @return: {number} 返回计算出来的分值
   */
int32_t game_calc_enemy_score(vector_type dir_a, vector_type dir_b)
{
  uint8_t close_sides = 0;
  uint8_t valid_area = 0;
  int32_t enemy_count_sum = 0;
  enemy_count_sum = dir_a.enemy_count + dir_b.enemy_count;
  valid_area = dir_a.enemy_search_area + dir_b.enemy_search_area;
  if (valid_area < 4)
  {
    return 5; //当前方向被堵死，返回一个很小的值
  }
  if (dir_a.enemy_search_area == dir_a.enemy_consecutive_count)
  {
    close_sides++;
  }
  if (dir_b.enemy_search_area == dir_b.enemy_consecutive_count)
  {
    close_sides++;
  }
  switch (dir_a.enemy_consecutive_count + dir_b.enemy_consecutive_count)
  {
  case 0:
    //以下是当前方向上分散棋子个数，间隔不超过1
    switch (enemy_count_sum)
    {
    case 0:
      return 0;
    case 1:
      return 100;
    case 2:
      return 200;
    case 3:
      return 400;
    default:
      return 2000;
    }
  case 1:
    //2连，2头空
    if (close_sides == 0)
    {
      //活跳三
      if (enemy_count_sum == 2)
      {
        return 3000;
      }
      //冲四
      if (enemy_count_sum >= 3)
      {
        return 4000;
      }
      //普通连2
      return 1000;
    } //眠二
    else
    {
      return 100;
    }
  case 2:
    //3连，2头空
    if (close_sides == 0)
    {
      //冲四
      if (enemy_count_sum >= 3)
      {
        return 4000;
      }
      //普通连3
      return 3000;
    } //眠三
    else
    {
      return 1000;
    }
  case 3:
    //活四
    if (close_sides == 0)
    {
      return 21000;
    } //冲四
    else
    {
      return 5000;
    }
  default:
    //敌方五连子了，返回仅次于己方五连子的最大得分
    return 10 * 10000;
  }
}
/**
 * game_count_chessman
 * @description:
 *   分析当前扫描到的点，并进行计数
 * @param: {uint8_t} x 当前扫描点的横坐标
 * @param: {uint8_t} y 当前扫描点的纵坐标
 * @param: {uint8_t} c 当前方向已经扫描的点数
 * @param: {vector_type *} vec 传入的当前扫描向量指针
 * @return: {类型}
 */
void game_count_chessman(uint8_t x, uint8_t y, uint8_t c, vector_type *vec)
{
  CHESSBOARD_POINT my_chessman = vec->me == WHO_BLACK ? CHESSMAN_BLACK : CHESSMAN_WHITE;
  CHESSBOARD_POINT enemy_chessman = vec->me == WHO_BLACK ? CHESSMAN_WHITE : CHESSMAN_BLACK;
  //如果当前扫描的不是第一个点，就判断当前点和上一个点是否都为空子
  if (c > 0)
  {
    //如果连续2空子，连续空子标志置1
    if (
        chessman_matrix[x][y] == CHESSMAN_EMPTY &&
        vec->previous_point == CHESSMAN_EMPTY)
    {
      vec->consecutive_empty_count_flag = 1;
    }
  }
  //扫到己方棋子，中断敌方棋子扫描
  if (chessman_matrix[x][y] == my_chessman)
  {
    vec->enemy_scan_flag = 0;
  }
  //扫到敌方棋子，中断己方棋子扫描
  else if (chessman_matrix[x][y] == enemy_chessman)
  {
    vec->my_scan_flag = 0;
  }
  //扫到空子，连续标志设置为0，对双方都一样
  else
  {
    vec->my_consecutive_flag = 0;
    vec->enemy_consecutive_flag = 0;
  }
  //如果我方允许扫描
  if (vec->my_scan_flag)
  {
    //扫描距离增加
    vec->my_search_area++;
    //对己方棋子计数
    if (
        chessman_matrix[x][y] == my_chessman &&
        !vec->consecutive_empty_count_flag)
    {
      vec->my_count++;
    }
    //对连续己方棋子计数
    if (vec->my_consecutive_flag)
    {
      vec->my_consecutive_count++;
    }
  }
  //如果敌方允许扫描
  if (vec->enemy_scan_flag)
  {
    //扫描区域增加
    vec->enemy_search_area++;
    //对敌方棋子计数
    if (
        chessman_matrix[x][y] == enemy_chessman &&
        !vec->consecutive_empty_count_flag)
    {
      vec->enemy_count++;
    }
    //对敌方连续棋子计数
    if (vec->enemy_consecutive_flag)
    {
      vec->enemy_consecutive_count++;
    }
  }
  //保存当前点
  vec->previous_point = chessman_matrix[x][y];
}
/**
 * game_scan_dir
 * @description:
 *   对一个空点的一个方向进行扫描，得到这个方向上的棋子信息
 * @param: {uint8_t} x 当前空点的横坐标
 * @param: {uint8_t} y 当前空点的纵坐标
 * @param: {WHO} me 我方棋子类型
 * @param: {dir_type} dir 扫描的方向
 * @return: {类型}
 */
vector_type game_scan_dir(uint8_t x, uint8_t y, WHO me, dir_type dir)
{
  //当前向量
  vector_type vec =
      {
          //区间内我方棋子总个数，遇到连续2空子停止计数 = 0
          .my_count = 0,
          //区间内我方连续棋子个数 = 0
          .my_consecutive_count = 0,
          //一个方向的扫描距离，最大四格，到敌方棋子、棋盘边缘截止 = 0
          .my_search_area = 0,
          //区间内敌方棋子总个数，遇到连续2空子停止计数 = 0
          .enemy_count = 0,
          //区间内敌方连续棋子个数 = 0
          .enemy_consecutive_count = 0,
          //一个方向的扫描距离，最大四格，到我方棋子、棋盘边缘截止 = 0
          .enemy_search_area = 0,
          //我方棋子连续标志 = 1
          .my_consecutive_flag = 1,
          //敌方棋子连续标志 = 1
          .enemy_consecutive_flag = 1,
          //我方继续扫描标志 = 1
          .my_scan_flag = 1,
          //敌方继续扫描标志 = 1
          .enemy_scan_flag = 1,
          //连续空子计数标志 = 0
          .consecutive_empty_count_flag = 0,
          //上一个扫描到的点 = CHESSMAN_EMPTY
          .previous_point = CHESSMAN_EMPTY,
          //我方棋子类型
          .me = me,
          //敌方棋子类型
          .enemy = me == WHO_BLACK ? WHO_WHITE : WHO_BLACK,
          //搜索的方向
          .dir = dir,
          //当前点横坐标
          .x = x,
          //当前点纵坐标
          .y = y};
  switch (dir)
  {
  case LT:
    for (
        int8_t i = x - 1, j = y - 1, c = 0;
        i >= 0 && j >= 0 && c < 4;
        i--, j--, c++)
    {
      game_count_chessman(i, j, c, &vec);
    }
    break;
  case T:
    for (int8_t i = x, j = y - 1, c = 0; j >= 0 && c < 4; j--, c++)
    {
      game_count_chessman(i, j, c, &vec);
    }
    break;
  case RT:
    for (
        int8_t i = x + 1, j = y - 1, c = 0;
        i < 15 && j >= 0 && c < 4;
        i++, j--, c++)
    {
      game_count_chessman(i, j, c, &vec);
    }
    break;
  case L:
    for (int8_t i = x - 1, j = y, c = 0; i >= 0 && c < 4; i--, c++)
    {
      game_count_chessman(i, j, c, &vec);
    }
    break;
  case R:
    for (
        int8_t i = x + 1, j = y, c = 0;
        i < 15 && c < 4;
        i++, c++)
    {
      game_count_chessman(i, j, c, &vec);
    }
    break;
  case LB:
    for (
        int8_t i = x - 1, j = y + 1, c = 0;
        i >= 0 && j < 15 && c < 4;
        i--, j++, c++)
    {
      game_count_chessman(i, j, c, &vec);
    }
    break;
  case B:
    for (
        int8_t i = x, j = y + 1, c = 0;
        j < 15 && c < 4;
        j++, c++)
    {
      game_count_chessman(i, j, c, &vec);
    }
    break;
  case RB:
    for (
        int8_t i = x + 1, j = y + 1, c = 0;
        i < 15 && j < 15 && c < 4;
        i++, j++, c++)
    {
      game_count_chessman(i, j, c, &vec);
    }
    break;
  default:
    break;
  }
  return vec;
}

/**
   * calc_left_to_right_score
   * @description:
   *   计算左到右方向上的分值
   * @return: {int32_t} 返回计算出来的分值，己方和敌方分数之和
   */
int32_t calc_left_to_right_score(vector_type dir_left, vector_type dir_right)
{
  return (
      game_calc_my_score(dir_left, dir_right) +
      game_calc_enemy_score(dir_left, dir_right));
}
/**
   * calc_left_top_to_right_bottom_score
   * @description:
   *   计算左上到右下方向上的分值
   * @return: {int32_t} 返回计算出来的分值，己方和敌方分数之和
   */
int32_t calc_left_top_to_right_bottom_score(vector_type dir_left_top, vector_type dir_right_bottom)
{
  return (
      game_calc_my_score(dir_left_top, dir_right_bottom) +
      game_calc_enemy_score(dir_left_top, dir_right_bottom));
}
/**
   * calc_top_to_bottom_score
   * @description:
   *   计算上到下方向上的分值
   * @return: {int32_t} 返回计算出来的分值，己方和敌方分数之和
   */
int32_t calc_top_to_bottom_score(vector_type dir_top, vector_type dir_bottom)
{
  return (
      game_calc_my_score(dir_top, dir_bottom) +
      game_calc_enemy_score(dir_top, dir_bottom));
}
/**
   * calc_right_top_to_left_bottom_score
   * @description:
   *   计算右上到左下方向上的分值
   * @return: {int32_t} 返回计算出来的分值，己方和敌方分数之和
   */
int32_t calc_right_top_to_left_bottom_score(vector_type dir_right_top, vector_type dir_left_bottom)
{
  return (
      game_calc_my_score(dir_right_top, dir_left_bottom) +
      game_calc_enemy_score(dir_right_top, dir_left_bottom));
}

int32_t calc_score_sum(
    vector_type dir_left_top,
    vector_type dir_top,
    vector_type dir_right_top,
    vector_type dir_left,
    vector_type dir_right,
    vector_type dir_left_bottom,
    vector_type dir_bottom,
    vector_type dir_right_bottom)
{
  int32_t scoreSum =
      calc_left_to_right_score(dir_left, dir_right) +
      calc_left_top_to_right_bottom_score(dir_left_top, dir_right_bottom) +
      calc_top_to_bottom_score(dir_top, dir_bottom) +
      calc_right_top_to_left_bottom_score(dir_right_top, dir_left_bottom);
  return scoreSum;
}

score_point_type calc_empty_point_score(uint8_t x, uint8_t y, WHO who_am_i)
{
  //TODO 计算分数
  score_point_type score_point;
  vector_type dir_left_top     = game_scan_dir(x, y, who_am_i, LT);
  vector_type dir_top          = game_scan_dir(x, y, who_am_i, T);
  vector_type dir_right_top    = game_scan_dir(x, y, who_am_i, RT);
  vector_type dir_left         = game_scan_dir(x, y, who_am_i, L);
  vector_type dir_right        = game_scan_dir(x, y, who_am_i, R);
  vector_type dir_left_bottom  = game_scan_dir(x, y, who_am_i, LB);
  vector_type dir_bottom       = game_scan_dir(x, y, who_am_i, B);
  vector_type dir_right_bottom = game_scan_dir(x, y, who_am_i, RB);
  int32_t position_score =
      (9.8995 - sqrt((x - 7) * (x - 7) + (y - 7) * (y - 7))) * 3 + (global_milliseconds % 5);
  score_point.score = position_score + calc_score_sum(
                                          dir_left_top,
                                          dir_top,
                                          dir_right_top,
                                          dir_left,
                                          dir_right,
                                          dir_left_bottom,
                                          dir_bottom,
                                          dir_right_bottom);
  score_point.x = x;
  score_point.y = y;
  return score_point;
}

cursor_type calc_placement_coordinate(WHO who_am_i)
{
  score_point_type high_score_point = {.x = -1, .y = -1, .score = -1};
  score_point_type point;
  cursor_type target_cursor;
  for (uint8_t i = 0; i < 15; i++)
  {
    for (uint8_t j = 0; j < 15; j++)
    {
      if (chessman_matrix[i][j] != CHESSMAN_EMPTY)
      {
        continue;
      }
      point = calc_empty_point_score(i, j, who_am_i);
      if (point.score > high_score_point.score)
      {
        high_score_point = point;
      }
    }
  }
  target_cursor.x = high_score_point.x;
  target_cursor.y = high_score_point.y;
  return target_cursor;
}

void game_mvp(void)
{
  cursor_type target_cursor;
  if (who_dropping == WHO_BLACK)
  {
    //下棋AI黑子
    target_cursor = calc_placement_coordinate(WHO_BLACK);
    game_drop_chessman(target_cursor.x, target_cursor.y);
  }
}
void game_pvm(void)
{
  cursor_type target_cursor;
  if (who_dropping == WHO_WHITE)
  {
    //下棋AI白子
    target_cursor = calc_placement_coordinate(WHO_WHITE);
    game_drop_chessman(target_cursor.x, target_cursor.y);
  }
}
void game_run(void)
{
  if (playing != PLAYING)
    return;
  winner = game_who_is_winner();
  if (winner != WINNER_NONE)
  {
    playing = STOPPING;
    page = GAME_PAGE_FINISH_MENU;
  }
  switch (game_mode)
  {
  case MVP:
    game_mvp();
    break;
  case PVM:
    game_pvm();
    break;
  case PVP:
  default:
    break;
  }
  if (game_mode != PVP)
  {
    winner = game_who_is_winner();
    if (winner != WINNER_NONE)
    {
      playing = STOPPING;
      page = GAME_PAGE_FINISH_MENU;
    }
  }
}

void game_left_cursor_x_increase(void)
{
  if (who_dropping != WHO_BLACK && game_mode == PVP)
    return;
  if (who_dropping == WHO_BLACK && game_mode == MVP)
    return;
  if (cursor.x < 14)
    cursor.x++;
  if (cursor.y > 14)
    cursor.x = 7;
}
void game_left_cursor_x_decrease(void)
{
  if (who_dropping != WHO_BLACK && game_mode == PVP)
    return;
  if (who_dropping == WHO_BLACK && game_mode == MVP)
    return;
  if (cursor.x > 0)
    cursor.x--;
  if (cursor.y > 14)
    cursor.x = 6;
}
void game_left_cursor_y_increase(void)
{
  if (who_dropping != WHO_BLACK && game_mode == PVP)
    return;
  if (who_dropping == WHO_BLACK && game_mode == MVP)
    return;
  if (cursor.y > 0)
    cursor.y--;
}
void game_left_cursor_y_decrease(void)
{
  if (who_dropping != WHO_BLACK && game_mode == PVP)
    return;
  if (who_dropping == WHO_BLACK && game_mode == MVP)
    return;
  if (cursor.y < 15)
    cursor.y++;
}
void game_right_cursor_x_increase(void)
{
  if (who_dropping != WHO_WHITE && game_mode == PVP)
    return;
  if (who_dropping == WHO_WHITE && game_mode == PVM)
    return;
  if (cursor.x < 14)
    cursor.x++;
  if (cursor.y > 14)
    cursor.x = 7;
}
void game_right_cursor_x_decrease(void)
{
  if (who_dropping != WHO_WHITE && game_mode == PVP)
    return;
  if (who_dropping == WHO_WHITE && game_mode == PVM)
    return;
  if (cursor.x > 0)
    cursor.x--;
  if (cursor.y > 14)
    cursor.x = 6;
}
void game_right_cursor_y_increase(void)
{
  if (who_dropping != WHO_WHITE && game_mode == PVP)
    return;
  if (who_dropping == WHO_WHITE && game_mode == PVM)
    return;
  if (cursor.y > 0)
    cursor.y--;
}
void game_right_cursor_y_decrease(void)
{
  if (who_dropping != WHO_WHITE && game_mode == PVP)
    return;
  if (who_dropping == WHO_WHITE && game_mode == PVM)
    return;
  if (cursor.y < 15)
    cursor.y++;
}
void game_start_menu_selection_up(void)
{
  if (start_menu_selection > 0)
    start_menu_selection--;
}
void game_start_menu_selection_down(void)
{
  if (start_menu_selection < START_MENU_SELECTION_MAX - 1)
    start_menu_selection++;
}
void game_finish_menu_selection_up(void)
{
  if (finish_menu_selection > 0)
    finish_menu_selection--;
}
void game_finish_menu_selection_down(void)
{
  if (finish_menu_selection < FINISH_MENU_SELECTION_MAX - 1)
    finish_menu_selection++;
}

game_type game =
    {
        .page = &page,
        .game_mode = &game_mode,
        .who_dropping = &who_dropping,
        .cursor = &cursor,
        .start_menu_selection = &start_menu_selection,
        .finish_menu_selection = &finish_menu_selection,
        .history_step = &history_step,
        .winner = &winner,
        .playing = &playing,
        .init = &game_init,
        .run = &game_run,
        .left_cursor_x_increase = &game_left_cursor_x_increase,
        .left_cursor_x_decrease = &game_left_cursor_x_decrease,
        .left_cursor_y_increase = &game_left_cursor_y_increase,
        .left_cursor_y_decrease = &game_left_cursor_y_decrease,
        .right_cursor_x_increase = &game_right_cursor_x_increase,
        .right_cursor_x_decrease = &game_right_cursor_x_decrease,
        .right_cursor_y_increase = &game_right_cursor_y_increase,
        .right_cursor_y_decrease = &game_right_cursor_y_decrease,

        .start_menu_selection_up = &game_start_menu_selection_up,
        .start_menu_selection_down = &game_start_menu_selection_down,
        .finish_menu_selection_up = &game_finish_menu_selection_up,
        .finish_menu_selection_down = &game_finish_menu_selection_down,
        .on_left_click = &game_on_left_click,
        .on_right_click = &game_on_right_click,
        .on_start_menu_click = game_on_start_menu_click,
        .on_finish_menu_click = game_on_finish_menu_click};
