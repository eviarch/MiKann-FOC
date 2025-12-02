#ifndef __COMMUNICATION_H
#define __COMMUNICATION_H

#include "main.h"
#include "usart.h"

#define COMM_HEAD_BYTE0             0xCC
#define COMM_HEAD_BYTE1             0x33

#define COMM_END_BYTE0              0x66
#define COMM_END_BYTE1              0x99

#define Comm_LinkStatus_PIN             BTE_Pin
#define Comm_LinkStatus_GPIO_PORT       BTE_GPIO_Port
#define Comm_GetLinkStatusPinLevel()    (HAL_GPIO_ReadPin(Comm_LinkStatus_GPIO_PORT, Comm_LinkStatus_PIN))      // PAin(15)  胃袋操作

#define Comm_USART                    huart2

void UART_Send_Task(void);


#endif /* __COMMUNICATION_H */