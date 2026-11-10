/**
 * @file PosPID.c
 * @author Zach (zachary-yue@qq.com)
 * 
 * @brief 固定时间间隔更新的 PID、PI、PD 控制器。
 * 
 * @version 0.1
 * @date 2025-12-01
 * 
 */
#include "PosPID.h"

/**
 * @file PosPID.c
 * @brief 位置式 PID、PI、PD 更新实现。
 */

/*------------------------------------------- PID ------------------------------------------*/

/**
 * @brief 位置式 PID 控制器初始化。
 * 
 * @param pid PID 结构体变量
 * @param kp 比例系数
 * @param ki 积分系数
 * @param kd 微分系数（微分项不除以时间因子）
 * @param intMax 积分限幅，传入正数最大值即可，会正负两头限幅
 * @param intDis 积分失能，传入正数最大值即可，会正负两头判断
 * @param delta_e_Max 微分最大值，传入正数最大值即可，会正负两头判断
 * @param uMax 总输出限幅，传入正数最大值即可，会正负两头限幅
 */
void PosPID_Init(PosPID_t *pid, float kp, float ki, float kd, float intMax, float intDis, float DeMax, float uMax)
{
    pid->e = 0.0f;
    pid->e_last = 0.0f;
    pid->e_int = 0.0f;
    pid->u = 0.0f;

    pid->Kp = kp;
    pid->Ki = ki;
    pid->Kd = kd;

    pid->e_int_max = ABS(intMax);
    pid->e_int_dis = ABS(intDis);
    pid->delta_e_max = ABS(DeMax);
    pid->u_max = ABS(uMax);
}


/**
 * @brief 位置式 PID 更新。
 * 
 * @param pid PID 结构体变量
 *
 * 误差绝对值超过 e_int_dis 时暂停积分；否则累加误差并做积分限幅。
 * 微分项取相邻两次误差的差值并做限幅，最终输出同样执行绝对值限幅。
 */
void PosPID_Update(PosPID_t *pid, float ek)
{
    float delta_e;
    pid->e = ek;
    delta_e = pid->e - pid->e_last;

    if (ABS(pid->e) > pid->e_int_dis) {
        pid->e_int = 0.0f;                          // 积分失能
    } else {
        pid->e_int += pid->e;                       // 积分
        LimAbsAsgn(pid->e_int, pid->e_int_max);     // 积分限幅
    }

    LimAbsAsgn(delta_e, pid->delta_e_max);          // 限制微分最大值

    pid->u = pid->Kp * pid->e + pid->Ki * pid->e_int + pid->Kd * delta_e;
    LimAbsAsgn(pid->u, pid->u_max);                 // 输出限幅

    pid->e_last = pid->e;
}


/**
 * @brief 清除 PID 的误差和积分状态，保留 Kp/Ki/Kd 与限幅参数。
 * 
 * @param pid The pid controller
 */
void PosPID_Clear(PosPID_t *pid)
{
    pid->e = 0.0f;
    pid->e_last = 0.0f;
    pid->e_int = 0.0f;
}

/*------------------------------------------- PD ------------------------------------------*/

/**
 * @brief 位置式 PD 控制器初始化。
 * 
 * @param pd PD 结构体变量
 * @param kp 比例系数
 * @param kd 微分系数（微分项不除以时间因子）
 * @param DeMax 微分最大值，传入正数最大值即可，会正负两头判断
 * @param uMax 总输出限幅，传入正数最大值即可，会正负两头限幅
 */
void PosPD_Init(PosPD_t *pd, float kp, float kd, float DeMax, float uMax)
{
    pd->e = 0.0f;
    pd->e_last = 0.0f;
    pd->u = 0.0f;

    pd->Kp = kp;
    pd->Kd = kd;

    pd->delta_e_max = ABS(DeMax);
    pd->u_max = ABS(uMax);
}


/**
 * @brief 位置式 PD 更新。
 * 
 * @param pd PD 结构体变量
 *
 * PD 不维护积分状态，仅由比例项和受限误差差分组成。
 */
void PosPD_Update(PosPD_t *pd, float ek)
{
    float delta_e;
    pd->e = ek;
    delta_e = pd->e - pd->e_last;

    LimAbsAsgn(delta_e, pd->delta_e_max);           // 限制微分最大值
    pd->u = pd->Kp * pd->e + pd->Kd * delta_e;
    LimAbsAsgn(pd->u, pd->u_max);    // 输出限幅

    pd->e_last = pd->e;
}


/**
 * @brief 清除 PD 的误差历史并保留控制器参数。
 * 
 * @param pd The pid controller
 */
void PosPD_Clear(PosPD_t *pd)
{
    pd->e = 0.0f;
    pd->e_last = 0.0f;
}

/*------------------------------------------- PI ------------------------------------------*/

/**
  * @brief 位置式 PI 控制器初始化。
  * 
  * @param pi PI 结构体变量
  * @param kp 比例系数
  * @param ki 积分系数
  * @param intMax 积分限幅，传入正数最大值即可，会正负两头限幅
  * @param intDis 积分失能，传入正数最大值即可，会正负两头判断
  * @param uMax 总输出限幅，传入正数最大值即可，会正负两头限幅
  */
void PosPI_Init(PosPI_t *pi, float kp, float ki, float intMax, float intDis, float uMax)
{
    pi->e = 0.0f;
    pi->e_int = 0.0f;
    pi->u = 0.0f;

    pi->Kp = kp;
    pi->Ki = ki;

    pi->e_int_max = ABS(intMax);
    pi->e_int_dis = ABS(intDis);
    pi->u_max = ABS(uMax);
}


/**
 * @brief 位置式 PI 更新。
 * 
 * @param pi PI 结构体变量
 * @param ek 当前控制误差。
 *
 * 与 PID 相同，误差过大时暂停积分；输出由比例项和受限积分项组成。
 */
void PosPI_Update(PosPI_t *pi, float ek)
{
    pi->e = ek;

    if (ABS(pi->e) > pi->e_int_dis) {
        pi->e_int = 0.0f;  // 积分失能
    } else {
        pi->e_int += pi->e;   // 积分
        LimAbsAsgn(pi->e_int, pi->e_int_max);    // 积分限幅
    }

    pi->u = pi->Kp * pi->e + pi->Ki * pi->e_int;
    LimAbsAsgn(pi->u, pi->u_max);    // 输出限幅
}


/**
 * @brief 清除 PI 的误差和积分状态，保留控制器参数。
 * 
 * @param pi The pi controller
 */
void PosPI_Clear(PosPI_t *pi)
{
    pi->e = 0.0f;
    pi->e_int = 0.0f;
}
