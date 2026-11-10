#ifndef __TASK_H
#define __TASK_H

/**
 * @file task.h
 * @brief 基于 TIM2 节拍的裸机周期任务接口。
 *
 * TIM2 每 100 us 进入一次中断，并通过 TaskExeFlag 通知主循环执行
 * TaskExe()。任务表的时间单位均是 100 us。
 */

#include "main.h"   // 添加到工程里之后修改为 main.h

/* 由 TIM2 中断置位、由主循环读取并清除的调度标志。 */
extern volatile u8 TaskExeFlag;

/* 任务节拍计数，每 100 us 增加 1，供控制算法计算时间间隔。 */
extern volatile u32 time_now;

#define SET_TaskExeFlag()       (TaskExeFlag = 1)
#define CLR_TaskExeFlag()       (TaskExeFlag = 0)
#define GET_TaskExeFlag()       (TaskExeFlag)

/* 将 100 us 节拍数换算为秒的缩放因子。 */
#define TIME_FACTOR     10000.0f        //  时间因子，例如: Time_task 以 100us 周期执行，则需要乘 10000.0f 变为 1s

/**
 * @brief 执行所有已经到达执行周期的任务。
 */
void TaskExe(void);

/**
 * @brief 初始化显示和控制模块。
 */
void Task_Init(void);

#endif // !__TASK_H
