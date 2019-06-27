#pragma once
#ifndef __TASK_UART_HEADER_GUARD__
#define __TASK_UART_HEADER_GUARD__
#include <stddef.h>
#include <shell.h>
void task_uart_lanch(size_t stack,size_t priority,shell_t* shell);
void task_uart_process(void*);

#endif 