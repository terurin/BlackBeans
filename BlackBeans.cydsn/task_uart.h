#pragma once
#ifndef __TASK_UART_HEADER_GUARD__
#define __TASK_UART_HEADER_GUARD__
#include <stddef.h>
    
void task_uart_init();
void task_uart_lanch(const char*name, size_t stack,size_t priority);
    
#endif 