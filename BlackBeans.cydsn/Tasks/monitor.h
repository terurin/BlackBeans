#pragma once
#ifndef __TASKS_MONITOR_HEADER_GUARD__
#define __TASKS_MONITOR_HEADER_GUARD__
#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <FreeRTOS.h>
#include <semphr.h>
#include <dsptype.h>
    
#define TASK_MONITOR_RECORD_SIZE 4
#define TASK_MONITOR_CHANNEL_SIZE 4    
    
enum task_monitor_channel{
    TASK_MONITOR_CHANNEL_VBAT=0,
    TASK_MONITOR_CHANNEL_M1C,
    TASK_MONITOR_CHANNEL_M2C,
    TASK_MONITOR_CHANNEL_M3C
};
typedef enum task_monitor_channel task_monitor_channel_t;

void task_monitor_init();
void task_monitor_lanch(const char* name,size_t size,size_t priority);

uint32_t task_monitor_read(task_monitor_channel_t);
q1516_t task_monitor_read_vbat_mv();
q1516_t task_monitor_mxc_a(task_monitor_channel_t ch);//ch!=0

#endif
    