
#include "FOC_Simulation.h"
#include "FOC_Math.h"


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


