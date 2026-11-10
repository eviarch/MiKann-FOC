#include "OLED.h"
#include "Font_EN.h"
#include "Font_CN.h"

/**
 * @file OLED.c
 * @brief OLED 显存绘制和 SSD1306/SSD1315 初始化序列。
 *
 * 页面绘制函数只修改 RAM 显存；OLED_Refresh() 才通过 I2C 发送整屏。
 */

/*---------------------------------------- Constants -----------------------------------------*/

#define OLED_CONTROLBYTE_CMD_STREAM     (0x00) // 多字节命令 0000 0000b
#define OLED_CONTROLBYTE_CMD_SINGLE     (0x80) // 单字节命令 1000 0000b
#define OLED_CONTROLBYTE_DATA           (0x40) // 传输数据   0100 0000b

#define OLED_DISPGRAPH_SIZE             ((u16)(8 * 128))

static uint8_t OLED_DispGraph[OLED_DISPGRAPH_SIZE];
BOOL OLED_IsInit = 0;      // <<< FIXED: replace BOOL with uint8_t

/*---------------------------------------- Function Declarations --------------------------------*/

void OLED_Clear(void);
void OLED_Refresh(void);

/*---------------------------------------- Library ---------------------------------------------*/

/*
 * 初始化控制器、设置寻址范围和电荷泵，最后打开显示。
 * 初始化序列通过阻塞式 I2C 写入，避免首次刷新与配置命令重叠。
 */
void OLED_Init(void)
{
    uint8_t init[] =
    {
        0xAE,               // 关闭显示（Display OFF）

        0x20, 0x00,         // 设置内存寻址模式 = 水平寻址模式（Horizontal addressing mode）

        0x21, 0x00, 0x7F,   // 设置列地址范围：0~127
        0x22, 0x00, 0x07,   // 设置页地址范围：0~7（共 8 页）

        0x40,               // 设置起始显示行 = 0

        // 段重映射与 COM 扫描方向设置
        0xA1,               // 段重映射（列地址 127 映射到 SEG0）
        0xC8,               // COM 扫描方向反转（从 COM[N−1] 扫描到 COM0）

        0xA8, 0x3F,         // 设置多路复用比例（MUX）= 63（对应 64 行）

        0xD3, 0x00,         // 设置显示偏移（Display Offset）= 0（不偏移）

        0x81, 0xCF,         // 设置对比度 = 0xCF（SSD1315 推荐值）

        0xA4,               // 显示内容跟随显存（不点亮所有像素）
        0xA6,               // 正常显示（非反相）

        0xD5, 0x80,         // 设置显示时钟分频（默认值即可）

        0xD9, 0xF1,         // 设置预充电周期（SSD1315 推荐值）

        0xDA, 0x12,         // 设置 COM 引脚硬件配置（SSD1315 默认配置，已确认正确）

        0xDB, 0x40,         // 设置 VCOMH 电压（SSD1315 推荐值）

        // --------- SSD1315 专用电荷泵配置 ---------
        0xAD, 0x30,         // 设置 DC-DC（SSD1315 独有的 0xAD 指令）
        0x8D, 0x14,         // 使能电荷泵
        // --------------------------------------------

        0xAF                // 打开显示（Display ON）

    };

    OLED_Clear();

    OLED_I2C_BlockWrite(OLED_CONTROLBYTE_CMD_STREAM, init, sizeof(init));

    OLED_IsInit = TRUE;
}

/* 将 1024 字节显存作为一块数据发送到 OLED。 */
void OLED_Refresh(void)
{
    HAL_I2C_Mem_Write_IT(&hi2c1, OLED_ADDR, OLED_CONTROLBYTE_DATA, I2C_MEMADD_SIZE_8BIT, OLED_DispGraph, OLED_DISPGRAPH_SIZE);
}


/* 仅清空本地显存，实际显示内容在下一次刷新时更新。 */
void OLED_Clear(void)
{
    memset(OLED_DispGraph, 0x00, OLED_DISPGRAPH_SIZE);
}

/*---------------------------------------- Char & String ---------------------------------------*/

/*
 * 输入行号从 1 开始，列号也按宏接口使用；函数内部转换为显存坐标。
 * ASCII 字形按 8×16 像素拆成上下两个 8 像素高的页。
 */
void OLED_ShowChar(uint8_t row, uint8_t col, char ch)
{
    uint8_t i;

    if (--row > 3 || --col > 15 || ch < 32 || ch > 127) return;
    ch -= 32;

    for (i = 0; i < 8; i++)
        OLED_DispGraph[2 * row * OLED_WIDTH + col * COL_STEP + i] = ascii_1608[(uint8_t)ch][i];

    for (i = 0; i < 8; i++)
        OLED_DispGraph[(2 * row + 1) * OLED_WIDTH + col * COL_STEP + i] = ascii_1608[(uint8_t)ch][i + 8];
}

/* 逐字符绘制，行尾自动换行，超出屏幕底部时停止。 */
void OLED_ShowString(uint8_t row, uint8_t col, const char* str)
{
    if (!str) return;

    if (--row > 3 || --col > 15) return;
    ++row; ++col;

    while (*str)
    {
        OLED_ShowChar(row, col, *str++);

        if (++col > 16)
        {
            col = 1;
            if (++row > 4) break;
        }
    }
}

/*---------------------------------------- Icons ----------------------------------------------*/

/* 16×16 图标占用两页，每页复制 16 字节。 */
void OLED_ShowIcon16(uint8_t row, uint8_t col, const uint8_t icon16[32])
{
    if (!icon16) return;
    if (--row > 3 || --col > 14) return;

    memcpy(OLED_DispGraph + (2 * row + 0) * OLED_WIDTH + col * COL_STEP, icon16 + 0, 16);
    memcpy(OLED_DispGraph + (2 * row + 1) * OLED_WIDTH + col * COL_STEP, icon16 + 16, 16);
}

#if Enable_Cn16Char
void OLED_ShowCnString16(uint8_t row, uint8_t col, char* CnString)
{
    uint8_t i;

    if (!CnString) return;

    while (*CnString)
    {
        for (i = 0; i < Cn16Char_SUMMARY; i++)
        {
            if (memcmp(CnString, Cn16Char[i].Index, 3) == 0)
            {
                OLED_ShowIcon16(row, col, Cn16Char[i].ImageData);
                break;
            }
        }

        CnString += 3;

        if ((col += 2) > 15)
        {
            col = 0;
            if (++row > 4) break;
        }
    }
}
#endif

#if Enable_Cn32Char
void OLED_ShowIcon32(uint8_t row, uint8_t col, const uint8_t icon32[128])
{
    if (!icon32) return;
    if (--row > 2 || --col > 12) return;

    memcpy(OLED_DispGraph + (2 * row + 0) * OLED_WIDTH + col * COL_STEP, icon32 + 0, 32);
    memcpy(OLED_DispGraph + (2 * row + 1) * OLED_WIDTH + col * COL_STEP, icon32 + 32, 32);
    memcpy(OLED_DispGraph + (2 * row + 2) * OLED_WIDTH + col * COL_STEP, icon32 + 64, 32);
    memcpy(OLED_DispGraph + (2 * row + 3) * OLED_WIDTH + col * COL_STEP, icon32 + 96, 32);
}

void OLED_ShowCnString32(uint8_t row, uint8_t col, char* CnString)
{
    uint8_t i;

    if (!CnString) return;

    while (*CnString)
    {
        for (i = 0; i < Cn32Char_SUMMARY; i++)
        {
            if (memcmp(CnString, Cn32Char[i].Index, 3) == 0)
            {
                OLED_ShowIcon32(row, col, Cn32Char[i].ImageData);
                break;
            }
        }

        CnString += 3;

        if ((col += 4) > 13)
        {
            col = 0;
            if (++row > 3) break;
        }
    }
}
#endif

/*---------------------------------------- Numbers ---------------------------------------------*/

/*
 * 从最低位向前生成十进制数字；左侧未使用的位按照 fill 参数补空格或 0。
 */
void OLED_ShowNum(uint8_t row, uint8_t col, int32_t num, uint8_t len, fill_t fill)
{
    if (--row > 3 || --col > 15 || len > 16) return;

    ++row; ++col;

    if (num < 0)
    {
        OLED_ShowChar(row, col, '-');
        num = -num;
        col++;
        len--;
    }

    while (len--)
    {
        OLED_ShowChar(row, col + len, (char)(num % 10 + '0'));
        num /= 10;
        if (num == 0) break;
    }

    while (len--)
    {
        OLED_ShowChar(row, col + len, (char)fill);
    }
}
