#include "file_uart.h"
#include <assert.h>
#include <project.h>
static size_t min(size_t x,size_t y){
    return x<y?x:y;
}


void file_uart_init(file_t*file ,size_t size){
    assert(file);
    file_base_init(file,size);
    file->object=NULL;
    file->write=(file_write_t)file_uart_write;
    file->read=(file_read_t)file_uart_read;
    file->flush=(file_flush_t)file_uart_flush;
    UART_Start();
}

size_t file_uart_write(void* context,const void* mem,size_t size){
    (void)context;
    UART_PutArray(mem,size);
    return size;
}

size_t file_uart_read(void* context,void* mem,size_t size){
    (void)context;
    const size_t receive_size=min(size,UART_GetRxBufferSize());
    for (size_t i=0;i<receive_size;i++){
        ((char*)mem)[i]=UART_GetChar();
    }
    return receive_size;
}
void file_uart_flush(void* context){
    (void)context;
    while(UART_GetTxBufferSize()>0);
    UART_ClearRxBuffer();
}