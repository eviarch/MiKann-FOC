/**
 * @file key.h
 * @author Zach (2063408575@qq.com)
 * 
 * @brief 按键模板，区分长短按。适用单个按键的情况。
 * 
 *        一些细节，如按键电平极性和长短按阈值等，在 .c 文件开头的宏定义处设置。
 * 
 * @version 0.1
 * @date 2025-07-08
 * 
 * @copyright Copyright (c) 2025
 * 
 */
#ifndef __KEY_SL_H
#define __KEY_SL_H

/**
 * @file key_SL.h
 * @brief 单按键短按/长按检测接口。
 */

#include "main.h"


/**
 * @brief 周期性扫描按键并分发短按或长按动作。
 *
 * 该函数应按固定周期调用；当前任务表中的调用周期为 20 ms。
 */
void KeySL_Task(void);

#endif // !__KEY_SL_H
