#ifndef __PAGE_MAIN_H
#define __PAGE_MAIN_H

/**
 * @file Page_Main.h
 * @brief 主页显示、菜单指针和按键回调接口。
 */

#include "Display.h"

#define PageMain_Ptr_MAX     ((u8)(PAGE_TOTAL - 1))   // 主页面的页面指针最大值


/*------------------------------- Exported Vars & Funcs -----------------------------*/

extern u8 Page_Main_Ptr;
extern u8 display_num;

/**
 * @brief 绘制主页静态文本和菜单标记。
 */
void Page_Main_Subtrate(void);

/**
 * @brief 更新主页动态测量值。
 */
void Page_Main_Task(void);


/*---------------------------------------------- User Determine -------------------------------------------*/

/**
 * @brief 处理主页短按，当前用于电角度零点校准。
 */
void Page_Main_Key_ShortPress(void);

/**
 * @brief 主页长按回调，当前未实现。
 */
void Page_Main_Key_LongPress(void);

/**
 * @brief 主页双击回调，当前未实现。
 */
void Page_Main_Key_DoublePress(void);

#endif // !__PAGE_MAIN_H
