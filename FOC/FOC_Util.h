#ifndef __FOC_UTILS_H
#define __FOC_UTILS_H

#include "main.h"

// FOC参数修改
#define PWM_CYCLE   1000.0f              // PWM的计数周期
#define VOLTAGE_POWER_SUPPLY 12.0f      // 电机的母线电压
#define VOLTAGE_LIMIT        12.0f      // 电机允许的最大电压

void setPWM(float Ua, float Ub, float Uc, float Udc);
void setPhaseVoltage(float Uq,float Ud, float angle_elctric, float zero_electric_angle);
void setTorque(float Uq,float angle_elctric);

#endif