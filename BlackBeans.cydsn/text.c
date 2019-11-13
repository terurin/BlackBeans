#include "text.h"

char* text_bits(char* buffer,unsigned int value,size_t size){
    size_t i;
    unsigned int mask;
    for (i=0;i<size-1;i++){
        mask= 1<<(size-i-1);
        buffer[i]=value&mask?'1':'0';
    }
    buffer[size-1]='\0';
    return buffer;
}