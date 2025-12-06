#ifndef __FOC_UTILS_H
#define __FOC_UTILS_H

#include "main.h"
#include "tim.h"

// FOC参数修改
#define PWM_CYCLE   htim1.Instance->ARR + 1              // PWM的计数周期
#define VOLTAGE_POWER_SUPPLY 12.6f      // 电机的母线电压
#define VOLTAGE_LIMIT        12.6f      // 电机允许的最大电压

void setPWM(float Ua, float Ub, float Uc, float Udc);
void setPhaseVoltage(float Uq,float Ud, float angle_elctric, float zero_electric_angle);
void setTorque(float Uq,float angle_elctric);

#endif