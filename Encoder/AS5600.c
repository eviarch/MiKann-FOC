
#include "AS5600.h"
#include "task.h"


#ifndef FLT_MAX
#define FLT_MAX         3.402823466e+38F
#endif

#ifndef INT32_MAX
#define INT32_MAX      0x7fffffffL
#endif


float encoder_degree = 0.0f;  // 存储每次采样后转换后的角度值(度)
u16 raw_angle;              // 存储读取到的原始角度值(0-4095)

Encoder_Turn_t encoder_turns = {0};
Encoder_Velocity_t encoder_velocity = {0};
Encoder_Velocity_Sin_Cos_t encoder_velocity_sc = {0};


/*********************I2C读取*************************/


/**
 * @brief 读取AS5600的原始角度值
 * @return 12位原始角度值(0-4095), 0xFFFF表示读取失败
 */
void AS5600_ReadRawAngle_I2C(void)
{
    u8 encoder_data[2];
    // 读取原始角度寄存器(连续读取2个字节)
    HAL_I2C_Mem_Read(&ENCODER_I2C, AS5600_ADDR, AS5600_RAW_ANGLE_H, I2C_MEMADD_SIZE_8BIT, encoder_data, 2, 1);

    raw_angle = (((u16)encoder_data[0] & 0x0F) << 8) |  encoder_data[1];
}

/*********************ADC读取*************************/


// AS5600 输出范围约为 0.04~0.96 VDD
#define AS5600_MIN_PERCENT   0.04f
#define AS5600_MAX_PERCENT   0.96f

void AS5600_ReadRawAngle_ADC(ADC_HandleTypeDef *hadc)
{
    // 启动 ADC
    HAL_ADC_Start(hadc);

    // 等待转换完成
    HAL_ADC_PollForConversion(hadc, 1);

    // 获取 ADC 原始值（0~4095）
    raw_angle = HAL_ADC_GetValue(hadc);

    // 停止 ADC
    HAL_ADC_Stop(hadc);
}


/*********************角度计算*************************/

/** 
 * @brief 将原始角度值转换为角度(度)
 * @param raw_angle 12位原始角度值(0-4095)
 * @return 角度值(0-360度)
 */
float AS5600_RawAngleToDegree(u16 raw_angle)
{
    // 4096对应360度
    return (float)raw_angle * 360.0f / 4096.0f;
}


/*********************角速度计算*************************/

//  自带了初始化判断, 只做了速度低通滤波
//  传统方法求解角速度
void Velocity_Calculate_Update(void)
{
    float angle_diff;
    float veloc_temp;
    if (!encoder_velocity.initialized)      // 初始化语句
    {
        encoder_velocity.initialized = 1;
        encoder_velocity.accel = 0;
        encoder_velocity.velocity = 0;
        encoder_velocity.angle_prev = encoder_degree;
        encoder_velocity.time_prev = time_now;
        LowPassFilter_Init(&encoder_velocity.velocity_lpf, 0.001, time_now);
        return;
    }

    float dt = (time_now - encoder_velocity.time_prev) / TIME_FACTOR;       // 秒
    if(dt < 0.0f || dt >= 0.3f)     // 超时重置，但未将time_now溢出纳入考虑范围
    {
        encoder_velocity.initialized = 1;
        encoder_velocity.accel = 0;
        encoder_velocity.velocity = 0;
        encoder_velocity.angle_prev = encoder_degree;
        encoder_velocity.time_prev = time_now;
        LowPassFilter_Init(&encoder_velocity.velocity_lpf, 0.001, time_now);
        return;
    }

    angle_diff = Lim_Ang_180(encoder_degree - encoder_velocity.angle_prev);
    if (ABS(angle_diff) < 0.2f)  angle_diff = 0.0f;
    veloc_temp = angle_diff / dt;

    LowPassFilter_Update(&encoder_velocity.velocity_lpf, veloc_temp, time_now);

    //  更新数据
    encoder_velocity.accel = angle_diff / dt - encoder_velocity.velocity;       // 此时encoder_velocity.velocity仍是上一次的数值
    encoder_velocity.velocity = angle_diff / dt;        //  此时encoder_velocity.velocity更新
    encoder_velocity.angle_prev = encoder_degree;
    encoder_velocity.time_prev = time_now;
}


//  自带了初始化判断, 只做了速度低通滤波
//  sin/cos 微分速度估计器
void Velocity_SinCos_Update(void)
{
    float veloc_temp;

    float s = _sin(DEG_TO_RAD(encoder_degree));
    float c = _cos(DEG_TO_RAD(encoder_degree));

    if (!encoder_velocity_sc.initialized) {
        encoder_velocity_sc.initialized = 1;
        encoder_velocity_sc.accel = 0;
        encoder_velocity_sc.velocity = 0;
        encoder_velocity_sc.s_prev = s;
        encoder_velocity_sc.c_prev = c;
        encoder_velocity_sc.time_prev = time_now;
        LowPassFilter_Init(&encoder_velocity_sc.velocity_lpf, 0.01, time_now);
        return;
    }

    float dt = (time_now - encoder_velocity_sc.time_prev) / TIME_FACTOR;

    if(dt < 0.0f || dt >= 0.3f)     // 超时重置，但未将time_now溢出纳入考虑范围
    {
        encoder_velocity_sc.initialized = 1;
        encoder_velocity_sc.accel = 0;
        encoder_velocity_sc.velocity = 0;
        encoder_velocity_sc.s_prev = s;
        encoder_velocity_sc.c_prev = c;
        encoder_velocity_sc.time_prev = time_now;
        LowPassFilter_Init(&encoder_velocity_sc.velocity_lpf, 0.01, time_now);
        return;
    }

    float ds = s - encoder_velocity_sc.s_prev;
    float dc = c - encoder_velocity_sc.c_prev;

    veloc_temp = RAD_TO_DEG((c * ds - s * dc) / dt);   // deg/s

    LowPassFilter_Update(&encoder_velocity_sc.velocity_lpf, veloc_temp, time_now);

    //  更新数据
    encoder_velocity_sc.accel = veloc_temp - encoder_velocity_sc.velocity;       // 此时encoder_velocity.velocity仍是上一次的数值
    encoder_velocity_sc.velocity = veloc_temp;        //  此时encoder_velocity.velocity更新
    encoder_velocity_sc.s_prev = s;
    encoder_velocity_sc.c_prev = c;
    encoder_velocity_sc.time_prev = time_now;
}


/*********************圈数计算*************************/

// 自带了初始化判断
void Turns_Count_Update(void)
{
    if (!encoder_turns.initialized)     // 初始化语句
    {
        encoder_turns.initialized = 1;
        encoder_turns.last_angle_deg = encoder_degree;
        encoder_turns.turns = 0;
        encoder_turns.total_angle_deg = encoder_degree;
        return;
    }

    float diff = encoder_degree - encoder_turns.last_angle_deg;
    // 判断跨零点
    if (diff > 270.0f) 
    {
        encoder_turns.turns -= 1;   // 方向：从 359° → 0°（逆向跨零点）
    } else if (diff < -270.0f) 
    {
        encoder_turns.turns += 1;   // 方向：从 0° → 359°（正向跨零点）
    }

    // 防止圈数溢出
    if (encoder_turns.turns >= INT32_MAX - 100)
    {
        encoder_turns.turns = 0;  // 防止溢出，重置圈数
    } else if (encoder_turns.turns <= -INT32_MAX + 100)
    {
        encoder_turns.turns = 0;  // 防止溢出，重置圈数
    }
    if(encoder_turns.total_angle_deg >= FLT_MAX - 36000.0f)
    {
        encoder_turns.total_angle_deg = encoder_degree; // 防止溢出，重置连续角度
    }
    

    encoder_turns.last_angle_deg = encoder_degree;
    // 计算连续角度
    encoder_turns.total_angle_deg = encoder_turns.turns * 360.0f + encoder_degree;
}


/*********************任务更新*************************/

void AS5600_Init(void)
{
    ;
}


void AS5600_Update(void)
{
    // AS5600_ReadRawAngle_I2C();
    AS5600_ReadRawAngle_ADC(&ENCODER_ADC);
    if(raw_angle != 0xFFFF)
    {
        encoder_degree = AS5600_RawAngleToDegree(raw_angle);
    }

    // Velocity_Calculate_Update();
    Turns_Count_Update();
}
