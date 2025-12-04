
#include "task.h"
#include "AS5600.h"
#include "Display.h"
#include "Communication.h"
#include "FOC.h"
#include "key_SL.h"

volatile u8 TaskExeFlag = 0;
volatile u32 time_now = 0;

void time_task(void)
{
    time_now++;
    if(time_now >= 60000)
    {
        time_now = 0;
    }
}

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
#define TASK_TOTAL      6

// 百微秒
Task_t Task[TASK_TOTAL] =
{
    { 1, 0, time_task },
    { 5, 0, AS5600_Update },
    { 5, 0, FOC_Task },
    // { 1000, 0, UART_Send_Task },
    { 100, 0, Display_Task },
    { 200, 0, KeySL_Task },
    
    { 10000, 0, count_task },
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

void Task_Init(void)
{
    Display_Init();
	FOC_Init();
}


void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
    if (htim->Instance == TIM2)
    {
        SET_TaskExeFlag();
    }
}