#pragma once
#ifndef __TASK_UART_HEADER_GUARD__
#define __TASK_UART_HEADER_GUARD__
#include <stddef.h>
    
void task_comm_init();
void task_comm_write(const void* mem,size_t size);

void task_comm_flush();

void task_comm_lanch(size_t stack,size_t priority);

void task_uart_process(void*);
void task_usb_process(void*);
#endif 