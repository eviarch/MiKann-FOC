
#include "FOC_App.h"
#include "FOC.h"
#include "FOC_Simulation.h"




/****************************************FOC任务执行***************************************************/

// FOC初始化函数
/*
 * PosPID_Init() 参数依次为：
 *   Kp、Ki、Kd、积分限幅、积分失能阈值、微分限幅、输出限幅。
 * 当前 Ki 和 Kd 按 5 ms 控制周期整定，修改 FOC_Task 周期时需要重新检查。
 */
void FOC_Init(void)
{
    PosPID_Init(
        &angleControl_loop_pid, 
        0.1f,
        0.1f / 5000.0f,
        0.01,
        5000.0f, 15.0f,
        2.0f,
        VOLTAGE_POWER_SUPPLY/2
    );
	// PosPI_Init(&velocityControl_loop_pid, 0.0025f, 6.0f / 50000.0f, 50000.0f, 300.0f, VOLTAGE_POWER_SUPPLY/2);
	LowPassFilter_Init(&angleControl_loop_filter, 0.1f);
	// LowPassFilter_Init(&velocityControl_loop_filter, 1.0f);

    // Ratchet_Simulation_Init();    // 低通不能给太狠了，微分项要起到消除震荡作用
    // Damp_Simulation_Init();
}


// 这里放FOC控制代码
/*
 * 当前默认模式是位置闭环，目标角固定为 45°。
 * 其他开环、速度和手感模拟入口保留在下方注释中，切换模式时只应启用
 * 一个主要控制路径，并确保对应控制器已经完成初始化。
 */
void FOC_Task(void)
{
	// Set_PWM_Frequency(&htim1, 1000);
	// velocityOpenloop(10.0f);  // 速度开环测试，10度每秒 
	angleControl_loop(45.0f);  // 位置闭环测试，目标位置45度
	// setTorque(VOLTAGE_POWER_SUPPLY / 2, _3PI_2);
	// velocityControl_loop(1000.0f);

    // Ratchet_Simulation_Update(0, 6);    // 低通不能给太狠了，微分项要起到消除震荡作用
    // Damp_Simulation_Update(-1);
}
