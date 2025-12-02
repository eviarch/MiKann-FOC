#ifndef __OLED_H
#define __OLED_H

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

typedef enum {
    FILL_BY_0 = '0',
    FILL_BY_SPACE = ' ',
} fill_t;

void OLED_Init(void);
void OLED_Refresh(void);

void OLED_Clear(void);
void OLED_ShowChar(u8 row, u8 col, char ch);
void OLED_ShowString(u8 row, u8 col, const char* str);

void OLED_ShowIcon16(u8 row, u8 col, const u8 icon16[32]);
void OLED_ShowCnString16(u8 row, u8 col, char* CnString);
void OLED_ShowNum(u8 row, u8 col, int32_t num, u8 len, fill_t fill);

#endif
