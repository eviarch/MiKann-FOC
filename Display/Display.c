/**
 * @file Display.c
 * @author Zach (zachary-yue@qq.com)
 * 
 * @brief OLED 显示初始化(Display_Init)、显示衬底信息(Display_Subtrate)和显示任务(Display_Task)。
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
#include "Display.h"

/**
 * @file Display.c
 * @brief 页面分发和 OLED 刷新调度。
 */

Page_enum ShowState = PAGE_Main;    // 当前显示页面，初始化给主页面


/*---------------------------------------------- User Determine -------------------------------------------*/

/**
 * @brief 显示衬底信息，在页面切换时执行。
 * @note  衬底信息就是一页里固定不用刷新的字符。
 * 
 */
void Display_Subtrate(void)
{
    OLED_Clear();

    switch (ShowState)
    {
        case PAGE_Main: Page_Main_Subtrate(); break;
        
        default: break;
    }
}


/**
 * @brief 显示任务，建议每 100 ms 执行一次。
 *
 * 当前任务表配置为 10 ms。每次先更新页面动态内容，再把整块显存提交给 OLED。
 */
void Display_Task(void)
{
    switch (ShowState)
    {
        case PAGE_Main: Page_Main_Task(); break;
        
        default: break;
    }

    OLED_Refresh();
}


/*-------------------------------------------------- Library -----------------------------------------------*/

/**
 * @brief 显示模块初始化，在工程初始化时执行。
 */
void Display_Init(void)
{
    OLED_Init();
    Display_Subtrate();
}
