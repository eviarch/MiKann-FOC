
#include "FOC.h"
#include "FOC_Util.h"
#include "FOC_Math.h"
#include "task.h"
#include "AS5600.h"
#include "tim.h"

/**
 * @file FOC.c
 * @brief 速度开环、位置闭环、速度闭环和电角度零点校准。
 *
 * 位置和速度控制器最终都生成 Uq。setPhaseVoltage() 再结合当前
 * 机械角对应的电角度完成反 Park、反 Clarke 和 PWM 输出。
 */

/* 编码器机械角与电机电角度之间的校准偏移，单位为弧度。 */
volatile float zero_electric_angle = 0.0f;

/****************************************速度开环( 力矩 )***************************************************/

u32 velocityOpenloop_time_stamp = 0;
float shaft_angle=0.0f;
// 速度开环控制函数
/*
 * 根据两次调用之间的软件时间计算虚拟机械角增量，再施加与虚拟
 * 电角度对齐的固定电压幅值。该模式不依赖位置反馈来判断误差，
 * 仅用于驱动方向、相序和基础旋转测试。
 */
void velocityOpenloop(float target_velocity)
{
	u32 now = time_now;
	float dt = (float)(now - velocityOpenloop_time_stamp) / TIME_FACTOR;		// 秒
	if (dt <= 0.0f)	dt = 1.0f;
	shaft_angle = _normalizeAngle(shaft_angle + DEG_TO_RAD(target_velocity)*dt);

  	// 最大只能设置为Uq = voltage_power_supply/2，否则ua,ub,uc会超出供电电压限幅
	setPhaseVoltage(VOLTAGE_POWER_SUPPLY/2,  0, _electricalAngle(shaft_angle, 7), zero_electric_angle);

	velocityOpenloop_time_stamp = now;
}

/****************************************位置闭环( 力矩-位置 )***************************************************/


PosPID_t angleControl_loop_pid;
LowPassFilter_t angleControl_loop_filter;
// 角度闭环控制函数
/*
 * 误差通过 Lim_Ang_180() 包裹到 -180°~180°，避免目标角与反馈角
 * 跨越零度时产生接近 360° 的错误误差。PID 输出限幅后经过一阶
 * 低通滤波，再作为 Uq 送入电压调制。
 */
void angleControl_loop(float target_angle)
{
	float Uq_out = 0.0f;
	
	PosPID_Update(&angleControl_loop_pid, Lim_Ang_180(target_angle - DIR * encoder_degree));
	
	Uq_out = Lim(angleControl_loop_pid.u, -VOLTAGE_POWER_SUPPLY/2, VOLTAGE_POWER_SUPPLY/2);

	LowPassFilter(&angleControl_loop_filter, Uq_out);  // 低通滤波，滤除突变

	setPhaseVoltage(angleControl_loop_filter.y_prev, 0.0f, _normalizeAngle(_electricalAngle(DIR * _normalizeAngle(DEG_TO_RAD(encoder_degree)), 7)), zero_electric_angle);
}


/****************************************速度闭环( 力矩-速度 )***************************************************/

PosPID_t velocityControl_loop_pid;
LowPassFilter_t velocityControl_loop_filter;
// 速度闭环控制函数
/*
 * 目标速度和反馈速度均使用 deg/s。当前函数计算速度误差并输出 Uq；
 * 滤波结果会更新 velocityControl_loop_filter，但本路径仍直接使用 PID 输出。
 */
void velocityControl_loop(float velocity)	// DEG
{
	PosPID_Update(&velocityControl_loop_pid, velocity - encoder_velocity_sc.velocity);
	LowPassFilter(&velocityControl_loop_filter, velocityControl_loop_pid.u);
	setPhaseVoltage(velocityControl_loop_pid.u, 0.0f,  _normalizeAngle(_electricalAngle(DIR * _normalizeAngle(DEG_TO_RAD(encoder_degree)), 7)), zero_electric_angle);
}

/****************************************电角度矫正***************************************************/

/*
 * 将定子磁场固定在 270° 电角度并等待转子稳定，然后读取机械角。
 * 通过 7 对极换算得到的电角度被保存为后续 Park 变换的零点。
 */
void Zero_Electric_Init(void)
{
	setTorque(VOLTAGE_POWER_SUPPLY/2, _3PI_2);
	HAL_Delay(1000);
	AS5600_Update();
	zero_electric_angle = _electricalAngle(DIR * _normalizeAngle(DEG_TO_RAD(encoder_degree)), 7);
	setTorque(0, _3PI_2);
}
