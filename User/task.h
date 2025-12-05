#ifndef __TASK_H
#define __TASK_H

#include "main.h"   // 添加到工程里之后修改为 main.h

extern volatile u8 TaskExeFlag;
extern volatile u32 time_now;

#define SET_TaskExeFlag()       (TaskExeFlag = 1)
#define CLR_TaskExeFlag()       (TaskExeFlag = 0)
#define GET_TaskExeFlag()       (TaskExeFlag)

#define TIME_FACTOR     10000.0f        //  时间因子，例如: Time_task 以 100us 周期执行，则需要乘 10000.0f 变为 1s

void TaskExe(void);
void Task_Init(void);

#endif // !__TASK_H
