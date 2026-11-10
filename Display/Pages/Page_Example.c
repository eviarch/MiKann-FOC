/**
 * @file Page_Example.c
 * @author Zach (zachary-yue@qq.com)
 * 
 * @brief 页面编写示例。使用时可以在编辑器里将 Example 字样统一替换成自己页面的名字。
 * 
 * @version 0.1
 * @date 2025-07-21
 * 
 */
#include "Page_Example.h"

/**
 * @file Page_Example.c
 * @brief 演示新页面应实现的最小函数集合。
 */

/* 静态衬底在进入页面或切换菜单时绘制一次。 */
void Page_Example_Subtrate(void)
{
    OLED_ShowString(1, 1, "Example");
}

/* 周期刷新函数可在每个显示周期更新数值、图标或选中状态。 */
void Page_Example_Task(void)
{
    ;
}


/* 以下三个回调分别对应短按、长按和双击预留动作。 */
void Page_Example_Key_ShortPress(void)
{
    ;
}

void Page_Example_Key_LongPress(void)
{
    ;
}

void Page_Example_Key_DoublePress(void)
{
    ;
}
