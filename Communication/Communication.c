
#include "Communication.h"
#include "AS5600.h"

/**
 * @file Communication.c
 * @brief USART2 链路状态采样和角度数据帧发送。
 */

/* 最近一次从 BTE 引脚读取到的连接状态。 */
static BOOL Comm_Linked = FALSE;

/**
 * @brief 从 GPIO 更新链路连接状态。
 */
void Comm_State_Update(void)
{
    Comm_Linked = Comm_GetLinkStatusPinLevel();
}

/**
 * @brief 获取最近一次采样的通信链路状态。
 * 
 * @return BOOL status (TRUE for linked and FALSE vice versa).
 */
BOOL Comm_GetLinkStatus(void)
{
    return Comm_Linked;
}

/**
 * @brief 向通信对端发送当前机械角。
 *
 * 目标格式为：CC 33 <angle-low> <angle-high> 66 99。
 * 当前 send_buf 长度和帧尾写入逻辑仍处于实验阶段，启用前需要确认
 * 帧长、字节序以及所有帧尾字节均能完整发送。
 */
void UART_Send_Task(void)
{
    Comm_State_Update();
    if (Comm_Linked == FALSE) return;
    u8 send_buf[4];

    send_buf[0] = COMM_HEAD_BYTE0;
    send_buf[1] = COMM_HEAD_BYTE1;

    *(u16*)(send_buf + 2) = (u16)(encoder_degree);

    send_buf[3] = COMM_END_BYTE0;

    HAL_UART_Transmit(&Comm_USART, send_buf, sizeof(send_buf), 10);
}
