
#include "Communication.h"
#include "AS5600.h"

static BOOL Comm_Linked = FALSE;

void Comm_State_Update(void)
{
    Comm_Linked = Comm_GetLinkStatusPinLevel();
}

/**
 * @brief Get BT link status.
 * 
 * @return BOOL status (TRUE for linked and FALSE vice versa).
 */
BOOL Comm_GetLinkStatus(void)
{
    return Comm_Linked;
}

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