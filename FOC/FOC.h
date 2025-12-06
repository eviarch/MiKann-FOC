#ifndef __FOC_H
#define __FOC_H

#include "main.h"
#include "PosPID.h"


#define DIR 1  // 方向控制，1为正向，-1为反向z


// 速度开环控制函数
extern u32 velocityOpenloop_time_stamp;
extern float shaft_angle;
void velocityOpenloop(float target_velocity);


// 角度闭环控制函数
extern PosPID_t angleControl_loop_pid;
extern LowPassFilter_t angleControl_loop_filter;
void angleControl_loop(float target_angle);


// 速度闭环控制函数
extern PosPID_t velocityControl_loop_pid;
extern LowPassFilter_t velocityControl_loop_filter;
void velocityControl_loop(float velocity);


// 电角度初始化
void Zero_Electric_Init(void);

#endif