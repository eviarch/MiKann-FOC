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


// 主页面中的功能选择指针。0 - 当前显示封面。
// >0 : 表示指向 ShowState 中的具体功能。每一个功能都对应一个页面。
u8 Page_Main_Ptr = 0;


/*---------------------------------------------- User Determine -------------------------------------------*/

/**
 * @brief 主页面显示的衬底
 * 
 */
void Page_Main_Subtrate(void)
{
    switch (Page_Main_Ptr)
    {
        case 0:
        {
            OLED_ShowCnString16(1, 1, "欢迎");
            OLED_ShowChar(1, 5, '!');
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
 */
void Page_Main_Task(void)
{
    if (Page_Main_Ptr == 0)
    {
        ;
    }
    else
    {
        OLED_ShowNum(1, 15, Page_Main_Ptr, 2, FILL_BY_0);
    }
}


/**
 * @brief 主页面下按键短按的反应。
 * 
 */
void Page_Main_Key_ShortPress(void)
{
    ;
}

/**
 * @brief 主页面下按键长按的反应。
 * 
 */
void Page_Main_Key_LongPress(void)
{
    ;
}

/**
 * @brief 主页面下按键双击的反应。
 * 
 */
void Page_Main_Key_DoublePress(void)
{
    ;
}
