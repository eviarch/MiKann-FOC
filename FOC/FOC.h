#ifndef __FOC_H
#define __FOC_H

#include "main.h"

#define DIR 1  // 方向控制，1为正向，-1为反向z


void FOC_Init(void);
void velocityOpenloop(float target_velocity);
void FOC_Task(void);
void Zero_Electric_Init(void);

#endif