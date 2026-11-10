/**
 * @file Page_Example.h
 * @author Zach (zachary-yue@qq.com)
 * 
 * @brief 页面编写示例。使用时可以在编辑器里将 Example 字样统一替换成自己页面的名字。
 * 
 * @version 0.1
 * @date 2025-07-21
 * 
 */
#ifndef __PAGE_Example_H
#define __PAGE_Example_H

/**
 * @file Page_Example.h
 * @brief 新页面实现的模板接口。
 */

#include "Display.h"

/* 页面切换和周期刷新时调用的显示函数。 */
void Page_Example_Subtrate(void);
void Page_Example_Task(void);

/* 页面激活时由按键模块调用的动作函数。 */
void Page_Example_Key_ShortPress(void);
void Page_Example_Key_LongPress(void);
void Page_Example_Key_DoublePress(void);

#endif // !__PAGE_Example_H
