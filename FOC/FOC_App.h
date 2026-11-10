#ifndef __FOC_APP_H
#define __FOC_APP_H

/**
 * @file FOC_App.h
 * @brief FOC 应用层初始化和周期任务入口。
 */

/**
 * @brief 初始化默认位置闭环所需的 PID 和滤波器状态。
 */
void FOC_Init(void);

/**
 * @brief 执行当前选择的电机控制模式。
 */
void FOC_Task(void);

#endif
