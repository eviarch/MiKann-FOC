#ifndef AS5600_H
#define AS5600_H

#include "main.h"

#define AS5600_ADDR         (0x36 << 1)    // AS5600的I2C地址
#define AS5600_RAW_ANGLE_H  0x0C    // 原始角度高字节寄存器
#define AS5600_RAW_ANGLE_L  0x0D    // 原始角度低字节寄存器
#define MAKEWORD(l, h)          ((WORD)(((BYTE)(l)) | ((WORD)((BYTE)(h))) << 8))

#define ENCODER_I2C     hi2c2  // 使用的I2C句柄

extern I2C_HandleTypeDef hi2c2;             // 你的 I2C 句柄（可改）

extern float encoder_degree;  // 存储转换后的角度值(度)

void AS5600_Update(void);

#endif // AS5600_H