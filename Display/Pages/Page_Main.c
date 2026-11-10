/**
 * @file Page_Main.c
 * @author Zach (zachary-yue@qq.com)
 * 
 * @brief 主页面的显示和按键功能。
 * 
 * @version 0.1
 * @date 2025-07-15
 * 
 */
#include "Page_Main.h"
#include "AS5600.h"
#include "FOC.h"
#include "key_SL.h"

/**
 * @file Page_Main.c
 * @brief 主页测量值显示和按键动作。
 */

// 主页面中的功能选择指针。0 - 当前显示封面。
// >0 : 表示指向 ShowState 中的具体功能。每一个功能都对应一个页面。
u8 Page_Main_Ptr = 0;
u8 key_count = 0;
u8 display_num = 0;


/*---------------------------------------------- User Determine -------------------------------------------*/

/**
 * @brief 主页面显示的衬底
 *
 * Page_Main_Ptr 为 0 时显示编码器信息栏；为 1~4 时显示菜单占位。
 */
void Page_Main_Subtrate(void)
{
    switch (Page_Main_Ptr)
    {
        case 0: 
        {
            OLED_ShowString(1, 1, "Turn: ");
			OLED_ShowString(2, 1, "Angle:");
			OLED_ShowString(3, 1, "Veloc:");
            OLED_ShowString(4, 1, "Accel:");
        } break;
        
        case 1:
        case 2:
        case 3:
        case 4:
        {
            OLED_ShowString(1, 1, "1.");
            OLED_ShowString(2, 1, "2.");
            OLED_ShowString(3, 1, "3.");
            OLED_ShowString(4, 1, "4.");
        } break;
    }
}


/**
 * @brief 主页面刷新的信息
 *
 * 左侧文本由静态衬底绘制，这里只更新数值区域，减少每帧字符重绘量。
 */
void Page_Main_Task(void)
{
    if (Page_Main_Ptr == 0)
    {
        OLED_ShowNum(1, 7, (int)encoder_turns.turns, 5, FILL_BY_SPACE);
        OLED_ShowNum(2, 7, (int)encoder_degree, 3, FILL_BY_SPACE);
        OLED_ShowNum(3, 7, (int)encoder_velocity_sc.velocity, 5, FILL_BY_SPACE);
        OLED_ShowNum(4, 7, (int)encoder_velocity_sc.accel, 5, FILL_BY_SPACE);
        OLED_ShowNum(1, 14, (int)display_num, 3, FILL_BY_0);
    }
    else
    {
        OLED_ShowNum(1, 15, Page_Main_Ptr, 2, FILL_BY_0);
    }
}


/**
 * @brief 主页面下按键短按的反应。
 *
 * 注意：Zero_Electric_Init() 会施加电压并阻塞约 1 s，调用前应确保
 * 电机可以自由转动且动力级接线正确。
 */
void Page_Main_Key_ShortPress(void)
{
    key_count++;
    Zero_Electric_Init();
}

/**
 * @brief 主页面下按键长按的反应。
 *
 * 保留为空，供后续菜单或参数编辑功能使用。
 */
void Page_Main_Key_LongPress(void)
{
    ;
}

/**
 * @brief 主页面下按键双击的反应。
 *
 * 当前按键驱动未产生双击事件，此接口仅预留。
 */
void Page_Main_Key_DoublePress(void)
{
    ;
}
