#ifndef __TASK_H
#define __TASK_H

#include "main.h"   // 添加到工程里之后修改为 main.h

extern volatile u8 TaskExeFlag;

#define SET_TaskExeFlag()       (TaskExeFlag = 1)
#define CLR_TaskExeFlag()       (TaskExeFlag = 0)
#define GET_TaskExeFlag()       (TaskExeFlag)

void TaskExe(void);

#endif // !__TASK_H
