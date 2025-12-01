/**
 * @file Display.h
 * @author Zach (zachary-yue@qq.com)
 * 
 * @brief OLED 显示初始化(Display_Init)、显示衬底信息(Display_Subtrate)和显示任务(Show_Task)。
 * 
 * @note  需要配置(STM32)：
 *        1. 使能 I2C。
 *        2. 使能 I2C Tx DMA。
 *        3. 需要开启 I2C Event Interrupt 和 DMA Stream Interrupt。
 *           （不知道为什么需要开启 I2C Event Interrupt，明明是 DMA 产生中断）
 * 
 *        Display 使用：
 *        1. 在 OLED.c 中修改一些底层操作。
 *        2. 调用 Display_Init 传入 I2C 句柄。
 *        3. 在 task.c 中添加 Display_Task，周期设置为 100 ms（或者大于 OLED 数据一次传输的时间均可）。
 *        4. 如果编写了页面，将每个页面的 Subtrate 和 Task 都分别添加到本文件的 Display_Subtrate
 *           和 Display_Task 中。
 * 
 * @version 0.1
 * @date 2025-07-15
 * 
 */
#ifndef __DISPLAY_H
#define __DISPLAY_H

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

void Display_Init(void);
void Display_Subtrate(void);
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
