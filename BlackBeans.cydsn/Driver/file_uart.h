#pragma once
#ifndef __FILE_UART_HEADER_GUARD__
#define __FILE_UART_HEADER_GUARD__

#include "file_base.h"
  
void file_init_uart(file_t*,size_t);
size_t file_uart_write(void*,const void*,size_t);
size_t file_uart_read(void*,void*,size_t);    
void file_uart_flush(void*);    
#endif
    