
#include "FOC_Simulation.h"
#include "FOC_Math.h"
#include "FOC.h"
#include "AS5600.h"


void Set_PWM_Frequency(TIM_HandleTypeDef *htim, u32 freq)
{
    u32 timer_clk = HAL_RCC_GetPCLK2Freq();  // 例如 TIM1/TIM8
    // 或 HAL_RCC_GetPCLK1Freq() 对 TIM2~TIM5
    // 若用了 x2（APB 时钟倍频），需要注意手册对应关系

    u32 psc = htim->Instance->PSC;
    u32 arr = (timer_clk / (freq * (psc + 1))) - 1;

    __HAL_TIM_DISABLE(htim);
    htim->Instance->ARR = arr;
    __HAL_TIM_ENABLE(htim);
}



RatchetRegionInfo_t getRatchetRegion(float angle, int n_sections)
{
    RatchetRegionInfo_t info;

    // 1. 归一化角度 0~360
    while (angle < 0) angle += 360.0f;
    while (angle >= 360.0f) angle -= 360.0f;

    // 2. 每个区域宽度
    float step = 360.0f / n_sections;

    // 3. 属于第几个区域
    int region = (int)(angle / step);

    // 4. 区域上下边界
    float lower = region * step;
    float upper = lower + step;

    // 5. 判断最近边界
    float d_lower = angle - lower;
    float d_upper = upper - angle;

    float nearest;
    if (d_lower <= d_upper)
        nearest = lower;
    else
        nearest = upper;

    // 写回结构体
    info.region_id    = region;
    info.lower_bound  = lower;
    info.upper_bound  = upper;
    info.nearest_bound = nearest;

    return info;
}

void Ratchet_Simulation_Init(void)
{
    LowPassFilter_Init(&angleControl_loop_filter, 0.8f);
    PosPID_Init(
        &angleControl_loop_pid, 
        0.1f,
        0.1f / 5000.0f,
        0.01f,
        5000.0f, 15.0f,
        2.0f,
        VOLTAGE_POWER_SUPPLY/2
    );
}

void Ratchet_Simulation_Update(s8 direction, u8 notch_number)
{
    RatchetRegionInfo_t r;
    r = getRatchetRegion(encoder_degree, notch_number);
    switch (direction)
    {
        case 0:
            angleControl_loop(r.nearest_bound);     //  顺时针和逆时针均可转动并形成挡位
            break;

        case 1:
            
            break;

        case -1:

            break;
        
        default:
            break;
    }
}

void Damp_Simulation_Init(void)
{
	LowPassFilter_Init(&velocityControl_loop_filter, 1.0f);
    PosPID_Init(
        &velocityControl_loop_pid, 
        0.01f,
        0.0f / 5000.0f,
        0.0f,
        5000.0f, 15.0f,
        2.0f,
        VOLTAGE_POWER_SUPPLY/2
    );
}



//  1：顺时针转动阻力
//  2：逆时针转动阻力
#define DAMP_INTENSITY        0.1f  // 阻力强度，数值越小阻力越大
#define VELOCITY_ENABLE     100.0f  // 速度使能，在一定速度以上才形成阻力
                                    // 只用输入正速度，在逆时针时已经加上负号了 
void Damp_Simulation_Update(s8 direction)
{
    switch (direction)
    {
        case 0:
            velocityControl_loop(DAMP_INTENSITY * encoder_velocity_sc.velocity);  
            break;
        
        case 1:
            if (encoder_velocity_sc.velocity > VELOCITY_ENABLE)
            {
                velocityControl_loop(DAMP_INTENSITY * encoder_velocity_sc.velocity);
            }
            else
            {
                velocityControl_loop(0.95f * encoder_velocity_sc.velocity);     // 给0.95是怕速度跑飞了
            }
            break;

        case -1:
            if (encoder_velocity_sc.velocity < - VELOCITY_ENABLE)
            {
                velocityControl_loop(DAMP_INTENSITY * encoder_velocity_sc.velocity);
            }
            else
            {
                velocityControl_loop(0.95f * encoder_velocity_sc.velocity);     // 给0.95是怕速度跑飞了
            }
            break;

        default:
            break;
    }
}