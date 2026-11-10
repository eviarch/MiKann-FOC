
#include "task.h"
#include "AS5600.h"
#include "Display.h"
#include "Communication.h"
#include "FOC_App.h"
#include "key_SL.h"

/**
 * @file task.c
 * @brief 周期任务调度和 100 us 软件时间基准。
 *
 * 调度采用“定时器中断置标志、主循环顺序执行”的协作式结构。
 * 所有任务共享同一个主循环上下文，因此单个任务阻塞会推迟后续任务。
 */

volatile u8 TaskExeFlag = 0;
volatile u32 time_now = 0;

/**
 * @brief 推进 100 us 软件时间基准。
 *
 * 该函数由调度器周期调用，而不是直接由 TIM2 中断调用。
 */
void time_task(void)
{
    time_now++;
    if(time_now >= UINT32_MAX - 1)
    {
        time_now = 0;
    }
}

/**
 * @brief 1 s 辅助任务。
 *
 * display_num 用于主页计数显示，同时翻转 LED 以观察主循环是否仍在运行。
 */
void count_task(void)
{
    display_num++;
    if (display_num >= 100)
    {
        display_num = 0;
    }
    HAL_GPIO_TogglePin(LED_GPIO_Port, LED_Pin);
}


/**
 * @brief 单个软件任务的描述。
 *
 * period 和 count 的单位均为 100 us。
 */
typedef struct
{
    const u16 period;          /* 任务执行周期，单位为 100 us。 */
    u16 count;                 /* 距上次执行经过的调度次数。 */
    void (*taskHook)(void);    /* 到期后调用的任务函数。 */
} Task_t;
#define TASK_TOTAL      7

// 百微秒
/*
 * 任务表按数组顺序执行：
 *   1   -> 100 us
 *   5   -> 500 us
 *   50  -> 5 ms
 *   500 -> 50 ms
 *   100 -> 10 ms
 *   200 -> 20 ms
 *   10000 -> 1 s
 */
Task_t Task[TASK_TOTAL] =
{
    { 1, 0, time_task },
    { 5, 0, AS5600_Update },
    { 50, 0, FOC_Task },
    { 500, 0, Velocity_SinCos_Update},
    // { 10000, 0, UART_Send_Task },
    { 100, 0, Display_Task },
    { 200, 0, KeySL_Task },
    
    { 10000, 0, count_task },
};

/**
 * @brief 检查并执行本拍到期的任务。
 *
 * 每个调度拍都会先给所有任务的计数加一；计数达到 period 后清零，
 * 然后再调用对应的任务函数。
 */
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

/**
 * @brief 初始化需要运行前就绪的功能模块。
 */
void Task_Init(void)
{
    Display_Init();
	FOC_Init();
}


/**
 * @brief HAL 定时器周期中断回调。
 *
 * 当前只把 TIM2 的 10 kHz 更新事件转换为主循环调度标志。
 */
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
    if (htim->Instance == TIM2)
    {
        SET_TaskExeFlag();
    }
}
