#ifndef __FOC_UTILS_H
#define __FOC_UTILS_H

/**
 * @file FOC_Util.h
 * @brief 三相电压、反 Park/Clarke 变换和 PWM 占空比接口。
 */

#include "main.h"
#include "tim.h"

// FOC参数修改
/* TIM1 的自动重装周期决定 PWM 计数范围。 */
#define PWM_CYCLE   htim1.Instance->ARR + 1              // PWM的计数周期

/* 当前实验平台的母线电压，需要与实际电源保持一致。 */
#define VOLTAGE_POWER_SUPPLY 12.6f      // 电机的母线电压

/* 单相 PWM 电压的绝对上限。 */
#define VOLTAGE_LIMIT        12.6f      // 电机允许的最大电压

/**
 * @brief 将三相电压转换为 TIM1 三路 PWM 比较值。
 *
 * @param Ua A 相目标电压。
 * @param Ub B 相目标电压。
 * @param Uc C 相目标电压。
 * @param Udc 直流母线电压；当前实现仍使用编译期配置。
 */
void setPWM(float Ua, float Ub, float Uc, float Udc);

/**
 * @brief 将 dq 电压参考转换为三相 PWM。
 *
 * @param Uq q 轴电压参考。
 * @param Ud d 轴电压参考；当前实现未使用。
 * @param angle_elctric 未校准的转子电角度，单位为弧度。
 * @param zero_electric_angle 电角度校准偏移，单位为弧度。
 */
void setPhaseVoltage(float Uq,float Ud, float angle_elctric, float zero_electric_angle);

/**
 * @brief 在指定电角度施加经过限幅的固定 q 轴电压。
 *
 * @param Uq q 轴电压幅值。
 * @param angle_elctric 目标电角度，单位为弧度。
 */
void setTorque(float Uq,float angle_elctric);

#endif
