
#ifndef FOC_UTILS_H
#define FOC_UTILS_H

#include <stdint.h>
#include <math.h>
#include "user_def.h"


/*------------------------------------- Exported Defines ----------------------------------*/

#define ABS(x)	        ((x) < 0 ? -(x) : (x))          // 绝对值
#define min(a, b)       ((a) < (b) ? (a) : (b))         // 较小值
#define max(a, b)       ((a) > (b) ? (a) : (b))         // 较大值
#define mid(a, b, c)    ( (a) > (b) ?                       \
                          ((b) > (c) ? (b) : min(a, c)) :   \
                          ((a) > (c) ? (a) : min(b, c)) )       // 中间值

// 限制范围（不赋值）
#define Lim(x, min, max)        ( (x) < (min) ? (min) : ((x) > (max) ? (max) : (x)) )

// 限制范围（同时赋值）
#define LimAsgn(x, min, max)    ( (x) < (min) ? ((x) = (min)) : ((x) > (max) ? ((x) = (max)) : (x)) )
#define LimAbsAsgn(x, abs)      LimAsgn(x, -(abs), (abs))


/*-----------------------------------------------------------------------------------------*/

#ifndef PI
#define PI                  (3.1415926535898f)          // 圆周率
#endif

#define DEG_TO_RAD(x)       ((x) * PI / 180.0f)         // 角度转换为弧度
#define RAD_TO_DEG(x)       ((x) * 180.0f / PI)         // 弧度转换为角度


// 将 -360 ~ -180 度和 +180 ~ +360 度的角度限制在 -180 ~ +180 度（不会对 x 赋值）
#define Lim_Ang_180(x)    ((x) > 180.0f ? ((x) - 360.0f) : ((x) < -180.0f ? ((x) + 360.0f) : (x)))

// 根据两点坐标，获取第二个点之于第一个点的角度
#define Get_Angle(x2, y2, x1, y1)	(RAD_TO_DEG(atan2((y2) - (y1), (x2) - (x1))))


extern const int sine_array[200];
#define SINETABLE_LEN               (sizeof(sine_array) / sizeof(int))


/* ---------- 基本工具宏 ---------- */

// 求符号（sign function）   <0 返回 -1，>0 返回 1，=0 返回 0
#define _sign(a)        (((a) < 0) ? -1 : (((a) > 0) ? 1 : 0))
// 对浮点数 x 执行四舍五入，返回 long（整数类型）
#define _round(x)       ((x) >= 0 ? (int32_t)((x)+0.5f) : (int32_t)((x)-0.5f))
// 计算 a 的平方  
#define _sqrt(a)        (_sqrtApprox(a))
#define _isset(a)       ((a) != (NOT_SET))
#define _UNUSED(v)      (void)(v)

/* ----------- 数学常数 ----------- */
#define _2_SQRT3        1.15470053838f
#define _SQRT3          1.73205080757f
#define _1_SQRT3        0.57735026919f
#define _SQRT3_2        0.86602540378f
#define _SQRT2          1.41421356237f
#define _120_D2R        2.09439510239f

#define _PI             3.14159265359f
#define _PI_2           1.57079632679f
#define _PI_3           1.0471975512f
#define _PI_6           0.52359877559f
#define _2PI            6.28318530718f
#define _3PI_2          4.71238898038f

#define NOT_SET         -12345.0f
#define _HIGH_IMPEDANCE 0
#define _HIGH_Z         _HIGH_IMPEDANCE
#define _ACTIVE         1


typedef struct {
    float d;
    float q;
} DQCurrent_s;

typedef struct {
    float d;
    float q;
} DQVoltage_s;

typedef struct {
    float a;
    float b;
    float c;
} PhaseCurrent_s;




/**
 * @brief 查表法正弦（输入范围：0~2π）
 */
float _sin(float a);

/**
 * @brief 查表法余弦（输入范围：0~2π）
 */
float _cos(float a);

/**
 * @brief 角度归一化：限制在 0~2π
 */
float _normalizeAngle(float angle);

/**
 * @brief 机械角 → 电角
 */
float _electricalAngle(float shaft_angle, int pole_pairs);

/**
 * @brief 快速平方根（Quake3）
 */
float _sqrtApprox(float value);

#endif /* FOC_UTILS_H */
