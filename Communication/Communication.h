#ifndef __COMMUNICATION_H
#define __COMMUNICATION_H

/**
 * @file Communication.h
 * @brief USART2 遥测接口和链路状态检测。
 *
 * 目标帧以两字节帧头开始、以两字节帧尾结束，角度值位于中间载荷。
 * 当前发送任务仍在实验阶段，默认未加入周期任务表。
 */

#include "main.h"
#include "usart.h"

/* 遥测帧头。 */
#define COMM_HEAD_BYTE0             0xCC
#define COMM_HEAD_BYTE1             0x33

/* 遥测帧尾。 */
#define COMM_END_BYTE0              0x66
#define COMM_END_BYTE1              0x99

/* PA15 作为蓝牙模块或上位机链路状态输入。 */
#define Comm_LinkStatus_PIN             BTE_Pin
#define Comm_LinkStatus_GPIO_PORT       BTE_GPIO_Port
#define Comm_GetLinkStatusPinLevel()    (HAL_GPIO_ReadPin(Comm_LinkStatus_GPIO_PORT, Comm_LinkStatus_PIN))      // PAin(15)  胃袋操作

/* 通信使用的 UART HAL 句柄。 */
#define Comm_USART                    huart2

/**
 * @brief 采样链路状态，并在连接有效时发送一帧编码器角度。
 */
void UART_Send_Task(void);


#endif /* __COMMUNICATION_H */
