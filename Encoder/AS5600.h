#ifndef AS5600_H
#define AS5600_H

#include "main.h"
#include "i2c.h"
#include "adc.h"

#define AS5600_ADDR         (0x36 << 1)    // AS5600的I2C地址
#define AS5600_RAW_ANGLE_H  0x0C    // 原始角度高字节寄存器
#define AS5600_RAW_ANGLE_L  0x0D    // 原始角度低字节寄存器
#define MAKEWORD(l, h)          ((WORD)(((BYTE)(l)) | ((WORD)((BYTE)(h))) << 8))

#define ENCODER_I2C     hi2c2  // 使用的I2C句柄
#define ENCODER_ADC     hadc1  // 使用的ADC句柄

typedef struct {
    float last_angle_deg;   // 上一次角度
    u32 turns;          // 圈数
    float total_angle_deg;  // 连续角度（圈数*360 + 当前角度）
    BOOL initialized;    // 是否首次运行
} Encoder_Turn_t;

extern Encoder_Turn_t encoder_turns;
extern float encoder_degree;  // 存储转换后的角度值(度)



void AS5600_Update(void);
void Revolution_Count_Update(void);

#endif // AS5600_H