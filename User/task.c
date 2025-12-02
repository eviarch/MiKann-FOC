
#include "task.h"
#include "AS5600.h"
#include "Display.h"
#include "Communication.h"

volatile u8 TaskExeFlag = 0;

void count_task(void)
{
    display_num++;
    if (display_num >= 100)
    {
        display_num = 0;
    }
    HAL_GPIO_TogglePin(LED_GPIO_Port, LED_Pin);
}


typedef struct
{
    const u16 period;
    u16 count;
    void (*taskHook)(void);
} Task_t;

#define TASK_TOTAL      4

Task_t Task[TASK_TOTAL] =
{
    { 60, 0, AS5600_Update },
    { 60, 0, UART_Send_Task },
    { 100, 0, Display_Task },
    
    { 1000, 0, count_task },
};

void TaskExe(void)
{
    u8 i = 0;
    for (; i < TASK_TOTAL; i++)
    {
        if (++Task[i].count >= Task[i].period)
        {
            Task[i].count = 0;
            if (Task[i].taskHook != NULL)
            {
                Task[i].taskHook();
            }
        }
    }
}


void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
    if (htim->Instance == TIM2)
    {
        SET_TaskExeFlag();
    }
}