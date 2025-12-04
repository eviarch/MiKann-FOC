
#include "AS5600.h"

#ifndef FLT_MAX
#define FLT_MAX         3.402823466e+38F
#endif

#ifndef UINT32_MAX
#define UINT32_MAX      0xffffffffUL
#endif

float encoder_degree = 0.0f;  // 存储转换后的角度值(度)
u16 raw_angle;              // 存储读取到的原始角度值(0-4095)
Encoder_Turn_t encoder_turns = {0};

/*********************I2C读取*************************/


/**
 * @brief 读取AS5600的原始角度值
 * @return 12位原始角度值(0-4095), 0xFFFF表示读取失败
 */
void AS5600_ReadRawAngle_I2C(void)
{
    u8 encoder_data[2];
    // 读取原始角度寄存器(连续读取2个字节)
    HAL_I2C_Mem_Read(&ENCODER_I2C, AS5600_ADDR, AS5600_RAW_ANGLE_H, I2C_MEMADD_SIZE_8BIT, encoder_data, 2, 1);

    raw_angle = (((u16)encoder_data[0] & 0x0F) << 8) |  encoder_data[1];
}

/*********************ADC读取*************************/


// AS5600 输出范围约为 0.04~0.96 VDD
#define AS5600_MIN_PERCENT   0.04f
#define AS5600_MAX_PERCENT   0.96f

void AS5600_ReadRawAngle_ADC(ADC_HandleTypeDef *hadc)
{
    // 启动 ADC
    HAL_ADC_Start(hadc);

    // 等待转换完成
    HAL_ADC_PollForConversion(hadc, 1);

    // 获取 ADC 原始值（0~4095）
    raw_angle = HAL_ADC_GetValue(hadc);

    // 停止 ADC
    HAL_ADC_Stop(hadc);
}


/*********************角度计算*************************/

/** 
 * @brief 将原始角度值转换为角度(度)
 * @param raw_angle 12位原始角度值(0-4095)
 * @return 角度值(0-360度)
 */
float AS5600_RawAngleToDegree(u16 raw_angle)
{
    // 4096对应360度
    return (float)raw_angle * 360.0f / 4096.0f;
}


/*********************圈数计算*************************/

void Revolution_Count_Update(void)
{
    if (!encoder_turns.initialized) {
        encoder_turns.initialized = 1;
        encoder_turns.last_angle_deg = encoder_degree;
        encoder_turns.turns = 0;
        encoder_turns.total_angle_deg = encoder_degree;
        return;
    }

    float diff = encoder_degree - encoder_turns.last_angle_deg;
    // 判断跨零点
    if (diff > 270.0f) {
        encoder_turns.turns -= 1;   // 方向：从 359° → 0°（逆向跨零点）
    } else if (diff < -270.0f) {
        encoder_turns.turns += 1;   // 方向：从 0° → 359°（正向跨零点）
    }

    // 防止圈数溢出
    if (encoder_turns.turns >= UINT32_MAX - 100)
    {
        encoder_turns.turns = 0;  // 防止溢出，重置圈数
    }
    if(encoder_turns.total_angle_deg >= FLT_MAX - 36000.0f)
    {
        encoder_turns.total_angle_deg = encoder_degree; // 防止溢出，重置连续角度
    }
    

    encoder_turns.last_angle_deg = encoder_degree;
    // 计算连续角度
    encoder_turns.total_angle_deg = encoder_turns.turns * 360.0f + encoder_degree;
}


/*********************任务更新*************************/


void AS5600_Update(void)
{
    // AS5600_ReadRawAngle_I2C();
    AS5600_ReadRawAngle_ADC(&ENCODER_ADC);
    if(raw_angle != 0xFFFF)
    {
        encoder_degree = AS5600_RawAngleToDegree(raw_angle);
        // 在此处处理angle_degree，例如存储或显示
    }
    else
    {
        encoder_degree = -888.8f; // 读取失败处理
    }

    Revolution_Count_Update();
}
