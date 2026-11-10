
#include "FOC_Util.h"
#include "FOC_Math.h"
#include "tim.h"

/**
 * @file FOC_Util.c
 * @brief 三相 PWM 调制和 dq/αβ/abc 坐标变换。
 */

/* 最近一次计算得到的三相占空比，便于调试观察。 */
float dc_a, dc_b, dc_c;

/* 反 Park 变换得到的静止坐标系电压。 */
float Ualpha=0.0f,Ubeta=0.0f;

/* 加入母线电压中点偏置后的三相电压。 */
volatile float Ua=0.0f,Ub=0.0f,Uc=0.0f;


/*
 * 先将相电压限制在有效范围，再按母线电压换算为 0~1 的占空比。
 * 最后把占空比映射到 TIM1 的比较寄存器 CCR1~CCR3。
 */
void setPWM(float Ua, float Ub, float Uc, float Udc)
{
	// 设置保护
	Ua = Lim(Ua, 0.0f, VOLTAGE_LIMIT);
	Ub = Lim(Ub, 0.0f, VOLTAGE_LIMIT);
	Uc = Lim(Uc, 0.0f, VOLTAGE_LIMIT);

	// 计算占空比
	// 限制占空比从0到1
	dc_a = Lim(Ua / VOLTAGE_POWER_SUPPLY, 0.0f , 1.0f );
	dc_b = Lim(Ub / VOLTAGE_POWER_SUPPLY, 0.0f , 1.0f );
	dc_c = Lim(Uc / VOLTAGE_POWER_SUPPLY, 0.0f , 1.0f );

	//写入PWM到PWM 0 1 2 通道
	htim1.Instance->CCR1 = (u16)(dc_a * PWM_CYCLE);
	htim1.Instance->CCR2 = (u16)(dc_b * PWM_CYCLE);
	htim1.Instance->CCR3 = (u16)(dc_c * PWM_CYCLE);
}


/*
 * 当前调用约定中 Uq 对齐 q 轴，Ud 对齐 d 轴。现有实现只使用 Uq，
 * 因此 Ualpha/Ubeta 不包含 Ud 分量。输出标幺值以母线电压中点为
 * 零电压参考，避免相电压出现负值。
 */
void setPhaseVoltage(float Uq,float Ud, float angle_elctric, float zero_electric_angle) {
	angle_elctric = _normalizeAngle(angle_elctric - zero_electric_angle);

	// 帕克逆变换
	Ualpha =  -Uq*_sin(angle_elctric); 
	Ubeta =   Uq*_cos(angle_elctric); 

	// 克拉克逆变换
	Ua = Ualpha + VOLTAGE_POWER_SUPPLY/2;
	Ub = (_sqrtApprox(3)*Ubeta-Ualpha)/2 + VOLTAGE_POWER_SUPPLY/2;
	Uc = (-Ualpha-_sqrtApprox(3)*Ubeta)/2 + VOLTAGE_POWER_SUPPLY/2;

 	// 设置PWM
 	setPWM(Ua,Ub,Uc, VOLTAGE_POWER_SUPPLY);
}


/*
 * 与 setPhaseVoltage() 类似，但使用绝对电角度且不使用编码器零点。
 * 该接口主要用于转子对齐和开环调试。
 */
void setTorque(float Uq,float angle_elctric) {
	LimAbsAsgn(Uq, VOLTAGE_POWER_SUPPLY/2);
	angle_elctric = _normalizeAngle(angle_elctric);

	// 帕克逆变换
	Ualpha =  -Uq*_sin(angle_elctric); 
	Ubeta =   Uq*_cos(angle_elctric); 

	// 克拉克逆变换
	Ua = Ualpha + VOLTAGE_POWER_SUPPLY/2;
	Ub = (_sqrtApprox(3)*Ubeta-Ualpha)/2 + VOLTAGE_POWER_SUPPLY/2;
	Uc = (-Ualpha-_sqrtApprox(3)*Ubeta)/2 + VOLTAGE_POWER_SUPPLY/2;
	setPWM(Ua,Ub,Uc, VOLTAGE_POWER_SUPPLY);
}
