#pragma once
#include <shell.h>
#include <stddef.h>
void task_usb_launch(size_t stack,size_t priority,shell_t* shell);
void task_usb_process();