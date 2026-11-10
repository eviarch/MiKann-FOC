#ifndef __OLED_H
#define __OLED_H

/**
 * @file OLED.h
 * @brief 128×64 I2C OLED 的 SSD1306/SSD1315 兼容驱动接口。
 */

#include "main.h"

/*---------------------------------------------- User Configuration -----------------------------------------*/
#define OLED_ADDR            (0x3C << 1)     // SSD1306 地址：0x3C 左移1位 = 0x78

extern I2C_HandleTypeDef hi2c1;             // 你的 I2C 句柄（可改）

#define OLED_I2C             hi2c1

// HAL I2C IT 模式（非阻塞）
#define OLED_I2C_Write(cmd, pdat, size) \
    HAL_I2C_Mem_Write_IT(&OLED_I2C, OLED_ADDR, (cmd), I2C_MEMADD_SIZE_8BIT, (uint8_t*)(pdat), (size))

// 阻塞等待发送完成
#define OLED_I2C_BlockWrite(cmd, pdat, size)        \
{                                                   \
    while (HAL_I2C_GetState(&OLED_I2C) != HAL_I2C_STATE_READY);  \
    HAL_I2C_Mem_Write_IT(&OLED_I2C, OLED_ADDR, (cmd), I2C_MEMADD_SIZE_8BIT, (uint8_t*)(pdat), (size)); \
    while (HAL_I2C_GetState(&OLED_I2C) != HAL_I2C_STATE_READY);  \
}

#define OLED_msDelay(ms)    HAL_Delay(ms)

/*-------------------------------------------------- Library ------------------------------------------------*/

#define OLED_WIDTH      128
#define OLED_HEIGHT     64

#define ROW_STEP        (OLED_WIDTH * 2)
#define COL_STEP        8

/* 数字显示时用于左侧前导字符的填充方式。 */
typedef enum {
    FILL_BY_0 = '0',
    FILL_BY_SPACE = ' ',
} fill_t;

/**
 * @brief 初始化 OLED 并清空内部显存。
 */
void OLED_Init(void);

/**
 * @brief 将完整显存通过 I2C 提交到 OLED。
 */
void OLED_Refresh(void);

/**
 * @brief 清空 RAM 显存，下一次 OLED_Refresh() 时生效。
 */
void OLED_Clear(void);

/**
 * @brief 在指定行列绘制一个 ASCII 字符。
 */
void OLED_ShowChar(u8 row, u8 col, char ch);

/**
 * @brief 在指定位置绘制以 '\0' 结尾的 ASCII 字符串。
 */
void OLED_ShowString(u8 row, u8 col, const char* str);

/* 图标、中文点阵和固定宽度数字绘制接口。 */
void OLED_ShowIcon16(u8 row, u8 col, const u8 icon16[32]);
void OLED_ShowCnString16(u8 row, u8 col, char* CnString);

/**
 * @brief 以固定宽度绘制 32 位有符号整数。
 */
void OLED_ShowNum(u8 row, u8 col, int32_t num, u8 len, fill_t fill);

#endif
