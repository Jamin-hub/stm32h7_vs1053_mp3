#ifndef __DEBUG_UART_H
#define __DEBUG_UART_H

#include "usart.h"      // 确保 huart1 和 HAL 库已包含
#include <stdio.h>

#ifdef DEBUG

// 安全的阻塞式串口打印（仅在 DEBUG 模式下生效）
#define u4_printf(fmt, ...) \
    do { \
        char debug_uart_buf[128]; \
        int len = snprintf(debug_uart_buf, sizeof(debug_uart_buf), fmt, ##__VA_ARGS__); \
        if (len > 0) { \
            HAL_UART_Transmit(&huart4, (uint8_t*)debug_uart_buf, \
                              (len < (int)sizeof(debug_uart_buf)) ? (uint16_t)len : (uint16_t)sizeof(debug_uart_buf)-1, \
                              HAL_MAX_DELAY); \
        } \
    } while(0)

#else
    // 非 DEBUG 模式：宏为空，无任何代码生成
    #define u1_printf(fmt, ...) ((void)0)
#endif

#endif // __DEBUG_UART_H