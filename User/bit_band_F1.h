/**
 * @file bit_band_F1.h
 * @author Zach (zachary-yue@qq.com)
 * 
 * @brief Macros of STM32F1 bit-banding.
 * 
 * @note  The header file should be included by 'user_def.h'.
 * 
 * @version 0.1
 * @date 2025-07-24
 * 
 */
#ifndef __BIT_BAND_F1_H
#define __BIT_BAND_F1_H

#ifdef __cplusplus
extern "C" {
#endif


#include "stm32f1xx.h"

// Bit-banding macros
#define BITBAND(addr, bitnum)   ((addr & 0xF0000000) + 0x2000000 + ((addr & 0xFFFFF) << 5) + (bitnum << 2))
#define BIT_ADDR(addr, bitnum)  (*(volatile unsigned long *)(BITBAND(addr, bitnum)))

// Define GPIO register offset (For STM32F1)
// (!) Note: Check the register offset before using bit-banding! ! !
// (!)       Make sure that the offset is compliant with those in the device's Reference Manual! ! !
#define GPIO_IDR_OFFSET         (0x08)
#define GPIO_ODR_OFFSET         (0x0C)

// Calculate GPIO Register Addresses
#define GPIOA_IDR_OFFSET        (GPIOA_BASE + GPIO_IDR_OFFSET)
#define GPIOB_IDR_OFFSET        (GPIOB_BASE + GPIO_IDR_OFFSET)
#define GPIOC_IDR_OFFSET        (GPIOC_BASE + GPIO_IDR_OFFSET)
#define GPIOD_IDR_OFFSET        (GPIOD_BASE + GPIO_IDR_OFFSET)
#define GPIOE_IDR_OFFSET        (GPIOE_BASE + GPIO_IDR_OFFSET)
// #define GPIOF_IDR_OFFSET        (GPIOF_BASE + GPIO_IDR_OFFSET)
// #define GPIOG_IDR_OFFSET        (GPIOG_BASE + GPIO_IDR_OFFSET)

#define GPIOA_ODR_OFFSET        (GPIOA_BASE + GPIO_ODR_OFFSET)
#define GPIOB_ODR_OFFSET        (GPIOB_BASE + GPIO_ODR_OFFSET)
#define GPIOC_ODR_OFFSET        (GPIOC_BASE + GPIO_ODR_OFFSET)
#define GPIOD_ODR_OFFSET        (GPIOD_BASE + GPIO_ODR_OFFSET)
#define GPIOE_ODR_OFFSET        (GPIOE_BASE + GPIO_ODR_OFFSET)
// #define GPIOF_ODR_OFFSET        (GPIOF_BASE + GPIO_ODR_OFFSET)
// #define GPIOG_ODR_OFFSET        (GPIOG_BASE + GPIO_ODR_OFFSET)

// Define macros for input/output operations
#define PBin(n)     BIT_ADDR(GPIOB_IDR_OFFSET, n)   // PB Input
#define PAin(n)     BIT_ADDR(GPIOA_IDR_OFFSET, n)   // PA Input
#define PCin(n)     BIT_ADDR(GPIOC_IDR_OFFSET, n)   // PC Input
#define PDin(n)     BIT_ADDR(GPIOD_IDR_OFFSET, n)   // PD Input
#define PEin(n)     BIT_ADDR(GPIOE_IDR_OFFSET, n)   // PE Input
// #define PFin(n)     BIT_ADDR(GPIOF_IDR_OFFSET, n)   // PF Input
// #define PGin(n)     BIT_ADDR(GPIOG_IDR_OFFSET, n)   // PG Input

#define PAout(n)    BIT_ADDR(GPIOA_ODR_OFFSET, n)    // PA Output
#define PBout(n)    BIT_ADDR(GPIOB_ODR_OFFSET, n)    // PB Output
#define PCout(n)    BIT_ADDR(GPIOC_ODR_OFFSET, n)    // PC Output
#define PDout(n)    BIT_ADDR(GPIOD_ODR_OFFSET, n)    // PD Output
#define PEout(n)    BIT_ADDR(GPIOE_ODR_OFFSET, n)    // PE Output
// #define PFout(n)    BIT_ADDR(GPIOF_ODR_OFFSET, n)    // PF Output
// #define PGout(n)    BIT_ADDR(GPIOG_ODR_OFFSET, n)    // PG Output


#ifdef __cplusplus
}
#endif

#endif // !__BIT_BAND_F1_H
