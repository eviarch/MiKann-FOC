#ifndef AS5600_H
#define AS5600_H

/**
 * @file AS5600.h
 * @brief AS5600 磁编码器读取、圈数累计和速度估计接口。
 *
 * 工程同时保留了 I2C 和模拟输出两种读取路径，但 AS5600_Update()
 * 当前使用 ADC 路径。角度单位统一为机械角度，速度单位为 deg/s。
 */

#include "main.h"
#include "i2c.h"
#include "adc.h"
#include "FOC_Math.h"

/* AS5600 寄存器地址和实际使用的 I2C 7 位地址。 */
#define AS5600_ADDR         (0x36 << 1)    // AS5600的I2C地址
#define AS5600_RAW_ANGLE_H  0x0C    // 原始角度高字节寄存器
#define AS5600_RAW_ANGLE_L  0x0D    // 原始角度低字节寄存器
#define MAKEWORD(l, h)          ((WORD)(((BYTE)(l)) | ((WORD)((BYTE)(h))) << 8))

/* 修改这两个宏即可切换编码器和模拟量所使用的 HAL 句柄。 */
#define ENCODER_I2C     hi2c2  // 使用的I2C句柄
#define ENCODER_ADC     hadc1  // 使用的ADC句柄


// 圈数和连续总角度计数结构体
/*
 * last_angle_deg 仅保存最近一次 0~360° 的机械角；
 * turns 记录跨零次数，total_angle_deg 用于表示可以超过一圈的连续角度。
 */
typedef struct {
    float last_angle_deg;    // 上一次角度
    s32 turns;               // 圈数
    float total_angle_deg;   // 连续角度（圈数*360 + 当前角度）(deg)
    BOOL initialized;        // 是否首次运行
} Encoder_Turn_t;


// 角速度和角加速度结构体
/*
 * 该结构供传统角度差分测速路径使用。当前 accel 字段保存的是
 * 相邻速度估计的变化量，尚未按时间间隔归一化为标准角加速度。
 */
typedef struct {
    float angle_prev;        // 上一次角度(rad)
    float velocity;          // 角速度(deg/s)
    float accel;             // 角加速度(deg/s^2)
    u32 time_prev;          // 上一时间(100us)
    LowPassFilter_t velocity_lpf; // 速度低通滤波
    BOOL initialized;        // 是否首次运行
} Encoder_Velocity_t;


/*
 * 基于 sin/cos 微分关系的测速状态。相比直接对角度做差分，
 * 该方法可以避免角度在 0°/360° 附近跳变造成的尖峰。
 */
typedef struct {
    float s_prev;           //  上一次sin值
    float c_prev;           //  上一次cos值
    float velocity;          // 角速度(deg/s)
    float accel;             // 角加速度(deg/s^2)
    u32 time_prev;          // 上一时间(100us)
    LowPassFilter_t velocity_lpf; // 速度低通滤波
    BOOL initialized;        // 是否首次运行
} Encoder_Velocity_Sin_Cos_t;


/* 编码器测量结果，由 AS5600_Update() 和测速任务共同更新。 */
extern Encoder_Turn_t encoder_turns;
extern Encoder_Velocity_t encoder_velocity;
extern Encoder_Velocity_Sin_Cos_t encoder_velocity_sc;
extern float encoder_degree;    // 存储转换后的角度值(度)
extern float machine_degree;    // 存储10次采样后中值滤波的角度



/**
 * @brief 读取一次编码器角度，并更新累计圈数。
 */
void AS5600_Update(void);

/**
 * @brief 使用传统角度差分和低通滤波估计角速度。
 */
void Velocity_Calculate_Update(void);

/**
 * @brief 使用 sin/cos 微分关系估计角速度。
 */
void Velocity_SinCos_Update(void);

/**
 * @brief 根据当前机械角判断是否跨零，并更新圈数。
 */
void Turns_Count_Update(void);

#endif // AS5600_H
