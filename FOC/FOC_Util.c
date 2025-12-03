
#include "FOC_Util.h"
#include "FOC_Math.h"
#include "tim.h"

float dc_a, dc_b, dc_c;
float zero_electric_angle = 0.0f;
float Ualpha=0.0f,Ubeta=0.0f;
volatile float Ua=0.0f,Ub=0.0f,Uc=0.0f;

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

void setPhaseVoltage(float Uq,float Ud, float angle_elctric) {
	angle_elctric = _normalizeAngle(angle_elctric + zero_electric_angle);

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
