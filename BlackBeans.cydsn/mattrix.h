#pragma once
#ifndef __MATTRIX_HEADER_GUARD__
#define __MATTRIX_HEADER_GUARD__
    
#include <dsptype.h>
#include <stddef.h>
//メモリ確保&開放
float** mattrix_float_new(size_t x,size_t y);
void mattrix_float_delete(float***,size_t x,size_t y);
int32_t** mattrix_int32_new(size_t x,size_t y);
void mattrix_int32_delete(int32_t***,size_t x,size_t y);
//初期化
float** mattrix_float_fill(float** dest,float value,size_t x,size_t y);
float** mattrix_float_unit(float** dest,float value,size_t x,size_t y);
//変換
q1516_t** mattrix_convert_float_to_q1516(q1516_t **dest,float** inst,size_t x,size_t y);
//y=Ax
q1516_t* mattrix_dot_vector(q1516_t * dest,const q1516_t** mattrix,const q1516_t *inst,size_t x,size_t y);
//~A
float** mattrix_float_inverse(float** dest,float** inst,size_t n);
//dump関数[低速]
char* mattrix_float_dump(char* result,size_t result_size,
    const float** mattrix,size_t x,size_t y);
#endif
    