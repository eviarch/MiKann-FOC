/**
 * @file key.c
 * @author Zach (2063408575@qq.com)
 * 
 * @brief 按键模板，区分长短按。适用单个按键的情况。
 * 
 *        一些细节，如按键电平极性和长短按阈值等，在 .c 文件开头的宏定义处设置。
 * 
 * @version 0.1
 * @date 2025-07-11
 * 
 * @copyright Copyright (c) 2025
 * 
 */
#include "key_SL.h"
#include "Display.h"

/*------------------------------------------- User Determine -------------------------------------------*/

#define GET_KEY_NOW()               (PBin(8))     // 当前按键的值

#define Key_IsPressed(__key__)      ((__key__) == Bit_SET)
#define Key_IsReleased(__key__)     ((__key__) == Bit_RESET)

#define Key_TaskPeriod              (20)    // KeyTask 20 ms 执行一次
#define Key_LP_THRES                (400 / Key_TaskPeriod)      // 长按阈值


static void Key_ShortPress(void)
{
    Page_Main_Key_ShortPress();
}


static void Key_LongPress(void)
{

}


/*---------------------------------------------- Library ----------------------------------------------*/

/**
 * @brief 按键周期性任务，20 ms 执行一次。
 * @note  区分长按、短按。
 * 
 */
void KeySL_Task(void)
{
    static u8 keyLast = Bit_RESET;
    static u8 keyState = 0, keyCount = 0;
    u8 keyNow = GET_KEY_NOW();

    switch (keyState)
    {
        case 0:
        {
            if (Key_IsPressed(keyNow) && Key_IsReleased(keyLast))
            {
                keyState = 1;
                keyCount = 0;
            }
        } break;

        case 1:
        {
            if (Key_IsPressed(keyNow))
            {
                if (++keyCount >= Key_LP_THRES)
                {
                    keyState = 0;
                    Key_LongPress();
                }
            }
            else
            {
                keyState = 0;
                Key_ShortPress();
            }
        } break;
    }

    keyLast = keyNow;
}
