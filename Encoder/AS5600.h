#ifndef AS5600_H
#define AS5600_H

#include "main.h"
#include "i2c.h"
#include "adc.h"
#include "FOC_Math.h"

#define AS5600_ADDR         (0x36 << 1)    // AS5600的I2C地址
#define AS5600_RAW_ANGLE_H  0x0C    // 原始角度高字节寄存器
#define AS5600_RAW_ANGLE_L  0x0D    // 原始角度低字节寄存器
#define MAKEWORD(l, h)          ((WORD)(((BYTE)(l)) | ((WORD)((BYTE)(h))) << 8))

#define ENCODER_I2C     hi2c2  // 使用的I2C句柄
#define ENCODER_ADC     hadc1  // 使用的ADC句柄


// 圈数和连续总角度计数结构体
typedef struct {
    float last_angle_deg;    // 上一次角度
    s32 turns;               // 圈数
    float total_angle_deg;   // 连续角度（圈数*360 + 当前角度）(deg)
    BOOL initialized;        // 是否首次运行
} Encoder_Turn_t;


// 角速度和角加速度结构体
typedef struct {
    float angle_prev;        // 上一次角度(rad)
    float velocity;          // 角速度(deg/s)
    float accel;             // 角加速度(deg/s^2)
    u32 time_prev;          // 上一时间(100us)
    LowPassFilter velocity_lpf; // 速度低通滤波
    BOOL initialized;        // 是否首次运行
} Encoder_Velocity_t;


typedef struct {
    float s_prev;           //  上一次sin值
    float c_prev;           //  上一次cos值
    float velocity;          // 角速度(deg/s)
    float accel;             // 角加速度(deg/s^2)
    u32 time_prev;          // 上一时间(100us)
    LowPassFilter velocity_lpf; // 速度低通滤波
    BOOL initialized;        // 是否首次运行
} Encoder_Velocity_Sin_Cos_t;


extern Encoder_Turn_t encoder_turns;
extern Encoder_Velocity_t encoder_velocity;
extern Encoder_Velocity_Sin_Cos_t encoder_velocity_sc;
extern float encoder_degree;    // 存储转换后的角度值(度)
extern float machine_degree;    // 存储10次采样后中值滤波的角度



void AS5600_Update(void);
void Velocity_Calculate_Update(void);
void Velocity_SinCos_Update(void);
void Turns_Count_Update(void);

#endif // AS5600_H