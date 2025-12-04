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

#include "FOC_Math.h"

/*------------------------------------------- PID ------------------------------------------*/

// 位置式 PID
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

void PosPID_Init(PosPID_t *pid, float kp, float ki, float kd, float intMax, float intDis, float DeMax, float uMax);
void PosPID_Update(PosPID_t *pid, float ek);
void PosPID_Clear(PosPID_t *pid);


/*------------------------------------------- PI ------------------------------------------*/

// 位置式 PI
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
