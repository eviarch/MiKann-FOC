/**
 * @file PosPID.h
 * @author Zach (zachary-yue@qq.com)
 * 
 * @brief 固定时间间隔更新的 PID、PI、PD 控制器。
 * 
 * @version 0.1
 * @date 2025-12-01
 * 
 */
#ifndef __PID_H
#define __PID_H

/**
 * @file PosPID.h
 * @brief 固定采样周期下使用的 PID、PI、PD 控制器。
 *
 * 微分项直接使用相邻误差差值和控制器系数，不额外除以采样时间；
 * 因此同一组参数只适用于对应的控制周期。
 */

#include "FOC_Math.h"

/*------------------------------------------- PID ------------------------------------------*/

// 位置式 PID
/* 控制器同时保存误差历史、积分状态、限幅参数和最近一次输出。 */
typedef struct PosPID_t {
    float e;            // 误差
    float e_last;       // 上次误差
    float e_int;        // 误差积分

    float Kp;           // 比例系数
    float Ki;           // 积分系数
    float Kd;           // 微分系数（微分项不除以时间因子）

    float e_int_max;    // 积分限幅（正数）
    float e_int_dis;    // 积分失能（正数）
    float delta_e_max;  // 误差限幅（正数）（当误差变化较大时认为是目标改变引起的）
    float u_max;        // 输出限幅（正数）

    float u;            // 输出
} PosPID_t;

/**
 * @brief 初始化位置式 PID 的内部状态和控制参数。
 */
void PosPID_Init(PosPID_t *pid, float kp, float ki, float kd, float intMax, float intDis, float DeMax, float uMax);

/**
 * @brief 输入当前误差并更新控制器输出 u。
 */
void PosPID_Update(PosPID_t *pid, float ek);

/**
 * @brief 清除误差、积分和上次误差，同时保留控制器参数。
 */
void PosPID_Clear(PosPID_t *pid);


/*------------------------------------------- PI ------------------------------------------*/

// 位置式 PI
/* PI 不保存微分历史，适用于速度环等需要较少状态的控制场景。 */
typedef struct PosPI_t {
    float e;            // 误差
    float e_int;        // 误差积分

    float Kp;           // 比例系数
    float Ki;           // 积分系数

    float e_int_max;    // 积分限幅（正数）
    float e_int_dis;    // 积分失能（正数）
    float u_max;        // 输出限幅（正数）

    float u;            // 输出
} PosPI_t;

void PosPI_Init(PosPI_t *pi, float kp, float ki, float intMax, float intDis, float uMax);
void PosPI_Update(PosPI_t *pi, float ek);
void PosPI_Clear(PosPI_t *pi);


/*------------------------------------------- PD ------------------------------------------*/

// 微分先行的位置式 PD
/* PD 不使用积分项，适合不希望累积稳态误差的场景。 */
typedef struct PosPD_t {
    float e;            // 误差
    float e_last;       // 上次误差

    float Kp;           // 比例系数
    float Kd;           // 微分系数（微分项不除以时间因子）

    float delta_e_max;  // 误差限幅（正数）（当误差变化较大时认为是目标改变引起的）
    float u_max;        // 输出限幅（正数）

    float u;            // 输出
} PosPD_t;

void PosPD_Init(PosPD_t *pd, float kp, float kd, float DeMax, float uMax);
void PosPD_Update(PosPD_t *pd, float ek);
void PosPD_Clear(PosPD_t *pd);

#endif // !__PID_H
