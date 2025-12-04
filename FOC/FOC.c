
#include "FOC.h"
#include "FOC_Util.h"
#include "FOC_Math.h"
#include "task.h"
#include "AS5600.h"
#include "PosPID.h"


u32 velocityOpenloop_time_stamp = 0;
float shaft_angle=0.0f;
PosPI_t angleControl_loop_pid;

// FOC初始化函数
void FOC_Init(void)
{
	PosPI_Init(&angleControl_loop_pid, 0.08f, 0.01f, 10.0f, 0.0f, VOLTAGE_POWER_SUPPLY/2);

	;// PWM初始化代码放这里
}


// 速度开环控制函数
void velocityOpenloop(float target_velocity)
{
	u32 now = time_now;
	float dt = (float)(now - velocityOpenloop_time_stamp) / 10000.0f;		// 秒
	if (dt <= 0.0f)	dt = 1.0f;
	shaft_angle = _normalizeAngle(shaft_angle + target_velocity*dt);

  	// 最大只能设置为Uq = voltage_power_supply/2，否则ua,ub,uc会超出供电电压限幅
	setPhaseVoltage(VOLTAGE_POWER_SUPPLY/2,  0, _electricalAngle(shaft_angle, 7));

	velocityOpenloop_time_stamp = now;
}

#define DIR 1  // 方向控制，1为正向，-1为反向z
// 这里放角度闭环控制代码
void angleControl_loop(float target_angle)
{
	float Uq_out = 0.0f;
	
	PosPI_Update(&angleControl_loop_pid, Lim_Ang_180(target_angle - DIR * encoder_degree));
	
	Uq_out = Lim(angleControl_loop_pid.u, -VOLTAGE_POWER_SUPPLY/2, VOLTAGE_POWER_SUPPLY/2);

	setPhaseVoltage(Uq_out, 0.0f, _normalizeAngle(_electricalAngle(DIR * DEG_TO_RAD(encoder_degree), 7)));
}

// 这里放FOC控制代码
void FOC_Task(void)
{
	// velocityOpenloop(10.0f);  // 速度开环测试，10rad/s
	angleControl_loop(45.0f);
}