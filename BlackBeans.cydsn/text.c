#include "text.h"

char* text_bits(char* buffer,unsigned int value,size_t size){
    size_t i;
    unsigned int mask=1;
    //後ろから書き込む
    char* it=buffer+size-1;
    *it ='\0';
    for (;it!=buffer-1;it--){
        *it = value&mask?'1':'0';
        mask<<=1;
    }
    return buffer;
}