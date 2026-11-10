/**
 * @file Display.h
 * @author Zach (zachary-yue@qq.com)
 * 
 * @brief OLED 显示初始化(Display_Init)、显示衬底信息(Display_Subtrate)和显示任务(Show_Task)。
 * 
 * @note  需要配置(STM32)：
 *        1. 使能 I2C。
 *        2. 当前 OLED 驱动使用 HAL I2C 中断发送，并非 DMA。
 *        3. 需要开启 I2C Event Interrupt 和 Error Interrupt。
 * 
 *        Display 使用：
 *        1. 在 OLED.c 中修改一些底层操作。
 *        2. 调用 Display_Init 传入 I2C 句柄。
 *        3. 在 task.c 中添加 Display_Task，当前周期为 10 ms。
 *        4. 如果编写了页面，将每个页面的 Subtrate 和 Task 都分别添加到本文件的 Display_Subtrate
 *           和 Display_Task 中。
 * 
 * @version 0.1
 * @date 2025-07-15
 * 
 */
#ifndef __DISPLAY_H
#define __DISPLAY_H

/**
 * @file Display.h
 * @brief OLED 显示初始化、页面衬底和周期刷新接口。
 */

#include "OLED.h"
#include "Pages.h"


/*---------------------------------------------- User Determine -------------------------------------------*/

/**
 * @brief 各个页面的序号（枚举）。
 * 
 */
typedef enum __page_enum
{
    PAGE_Main = 0,

    

    PAGE_TOTAL          // 注意这个 PAGE_TOTAL 是算上主页面之后的页面总数
} Page_enum;


/*-------------------------------------------------- Library -----------------------------------------------*/

extern Page_enum ShowState;    // 当前显示页面

/**
 * @brief 初始化 OLED 并绘制当前页面中不随时间变化的静态内容。
 */
void Display_Init(void);

/**
 * @brief 清屏并重绘当前页面的静态衬底。
 */
void Display_Subtrate(void);

/**
 * @brief 刷新当前页面的动态内容，并提交整屏显存。
 */
void Display_Task(void);

/**
 * @brief 回到主页面，在每个页面退出时可引用。
 *        操作包含：清屏，设置当前显示页面为主页面，显示衬底。
 */
#define ReturnPageMain()    {   \
    ShowState = PAGE_Main;      \
    Display_Subtrate();         \
}

#endif // !__DISPLAY_H
