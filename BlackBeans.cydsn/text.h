#pragma once
#ifndef __TEXT_HEADER_GAURD__
#define __TEXT_HEADER_GUARD__
    #include <stdint.h>
    #include <stddef.h>
    
    //[0,size-1)bitを表示する
    char* text_bits(char*,unsigned int value,size_t size);
#endif
    