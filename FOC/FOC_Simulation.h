#ifndef __FOC_SIMULATION_H
#define __FOC_SIMULATION_H

/**
 * @file FOC_Simulation.h
 * @brief 棘轮挡位和阻尼手感的实验接口。
 */

#include "main.h"
#include "FOC_Util.h"


/* 一个挡位分区的上下边界以及区域内距离最近的挡位。 */
typedef struct {
    int region_id;        // 落在哪个区域
    float lower_bound;    // 区域下边界
    float upper_bound;    // 区域上边界
    float nearest_bound;  // 最近的边界
} RatchetRegionInfo_t;


/**
 * @brief 临时修改定时器 PWM 频率。
 *
 * @param htim 目标定时器；当前函数假设使用 APB2 时钟。
 * @param freq 目标 PWM 频率，单位为 Hz。
 */
void Set_PWM_Frequency(TIM_HandleTypeDef *htim, u32 freq);

/**
 * @brief 初始化棘轮手感模式的位置控制器参数。
 */
void Ratchet_Simulation_Init(void);

/**
 * @brief 将转子吸附到最近的棘轮挡位。
 *
 * @param direction 方向选择；0 表示双向吸附，1/-1 分支目前预留。
 * @param notch_number 圆周挡位数量。
 */
void Ratchet_Simulation_Update(s8 direction, u8 notch_number);

/**
 * @brief 初始化阻尼手感模式的速度控制器参数。
 */
void Damp_Simulation_Init(void);

/**
 * @brief 根据速度和方向生成阻尼力矩参考。
 *
 * @param direction 0 表示双向阻尼，1 表示正向，-1 表示反向。
 */
void Damp_Simulation_Update(s8 direction);

#endif
