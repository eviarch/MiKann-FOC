
#ifndef FOC_UTILS_H
#define FOC_UTILS_H

#include <stdint.h>
#include <math.h>

/* ---------- 基本工具宏 ---------- */
#define _sign(a)        (((a) < 0) ? -1 : (((a) > 0) ? 1 : 0))
#define _round(x)       ((x) >= 0 ? (long)((x)+0.5f) : (long)((x)-0.5f))
#define _constrain(v,l,h) (((v)<(l))?(l):(((v)>(h))?(h):(v)))
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
    float a;
    float b;
    float c;
} PhaseCurrent_s;

typedef struct {
    float d;
    float q;
} DQVoltage_s;



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
