/**
 * @file def.h
 * @author Zach (2063408575@qq.com)
 * 
 * @brief 一些 defines 和 typedefs。此文件被 main.h 包含。
 * 
 * @version 0.1
 * @date 2025-07-08
 * 
 * @copyright Copyright (c) 2025
 * 
 */
#ifndef __USER_DEF_H
#define __USER_DEF_H

#include <string.h>
#include <stdio.h>

#include "stm32f1xx.h"

#include "bit_band_F1.h"

/*-------------------------------- Defines & Typedefs -------------------------------*/

/* 工程内部使用的短整型别名，统一控制模块的整数宽度。 */
typedef uint8_t u8;
typedef uint16_t u16;
typedef uint32_t u32;
typedef int8_t s8;
typedef int16_t s16;
typedef int32_t s32;

/* 布尔值沿用 u8，避免引入额外类型。 */
typedef u8 BOOL;

/* 位带和普通 GPIO 逻辑值。 */
#define Bit_RESET       ((u32)(0))
#define Bit_SET         ((u32)(1))

#ifndef TRUE
#define TRUE            ((u8)1)
#endif
#ifndef FALSE
#define FALSE           ((u8)0)
#endif

#ifndef SUCCESS
#define SUCCESS         ((u8)0)
#endif
#ifndef FAILED
#define FAILED          ((u8)1)
#endif

/* 显式标记未使用参数，避免编译器警告。 */
#define USER_UNUSED(x)  (void)(x)


#endif // !__USER_DEF_H
