
#include "task.h"

volatile u8 TaskExeFlag = 0;

void led_task(void)
{
    HAL_GPIO_TogglePin(LED_GPIO_Port, LED_Pin);
}


typedef struct
{
    const u16 period;
    u16 count;
    void (*taskHook)(void);
} Task_t;

#define TASK_TOTAL      1

Task_t Task[TASK_TOTAL] =
{
    { 1000, 0, led_task },
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