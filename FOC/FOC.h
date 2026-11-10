#ifndef __FOC_H
#define __FOC_H

/**
 * @file FOC.h
 * @brief 电机控制模式、PID 状态和电角度校准接口。
 *
 * 当前实现属于电压模式控制：控制器输出 Uq 电压幅值，没有相电流
 * 反馈和 Id/Iq 电流环。机械角使用度，电角度使用弧度。
 */

#include "main.h"
#include "PosPID.h"


/* 机械旋转方向；1 表示正向，-1 表示反向。 */
#define DIR 1  // 方向控制，1为正向，-1为反向z


// 速度开环控制函数
/* 开环速度控制使用的上一步时间戳，单位为 100 us 节拍。 */
extern u32 velocityOpenloop_time_stamp;

/* 速度开环模式中人为积分得到的机械角，单位为弧度。 */
extern float shaft_angle;

/**
 * @brief 按目标机械角速度推进虚拟电角度。
 *
 * @param target_velocity 目标机械角速度，单位为 deg/s。
 */
void velocityOpenloop(float target_velocity);


// 角度闭环控制函数
extern PosPID_t angleControl_loop_pid;
extern LowPassFilter_t angleControl_loop_filter;

/**
 * @brief 对机械角执行位置闭环控制。
 *
 * @param target_angle 目标机械角，单位为度。
 */
void angleControl_loop(float target_angle);


// 速度闭环控制函数
extern PosPID_t velocityControl_loop_pid;
extern LowPassFilter_t velocityControl_loop_filter;

/**
 * @brief 对机械角速度执行闭环控制。
 *
 * @param velocity 目标机械角速度，单位为 deg/s。
 */
void velocityControl_loop(float velocity);


// 电角度初始化
/**
 * @brief 施加固定电压使转子对齐，并记录编码器与电角度的零点关系。
 *
 * 该校准过程包含约 1 s 的阻塞等待，结果仅保存在 RAM 中。
 */
void Zero_Electric_Init(void);

#endif
