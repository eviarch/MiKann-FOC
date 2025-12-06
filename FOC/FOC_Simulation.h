#ifndef __FOC_SIMULATION_H
#define __FOC_SIMULATION_H

#include "main.h"
#include "FOC_Util.h"


typedef struct {
    int region_id;        // 落在哪个区域
    float lower_bound;    // 区域下边界
    float upper_bound;    // 区域上边界
    float nearest_bound;  // 最近的边界
} RatchetRegionInfo_t;


void Set_PWM_Frequency(TIM_HandleTypeDef *htim, u32 freq);

void Ratchet_Simulation_Init(void);
void Ratchet_Simulation_Update(s8 direction, u8 notch_number);

void Damp_Simulation_Init(void);
void Damp_Simulation_Update(s8 direction);

#endif