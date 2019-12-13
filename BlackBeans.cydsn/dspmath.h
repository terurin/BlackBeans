#pragma once
#ifndef __DSP_MATH_HEADER_GUARD__
#define __DSP_MATH_HEADER_GUARD__

    #include "dsptype.h"
    
    extern const q1516_t q1516_pi;
    extern const q1516_t q1516_2pi;
    //簡易計算用
    static inline q1516_t q1516_div(q1516_t x,q1516_t y){
        return ((int64_t)x<<16)/y;
    }
    static inline q1516_t q1516_mul(q1516_t x,q1516_t y){
        return (int64_t)x*y>>16;
    }
    
    static inline int32_t abs32(int32_t x){
        return x>=0?x:x;   
    }
    static inline int64_t abs64(int64_t x){
        return x>=0?x:x;   
    }
    
    uint32_t fact(uint32_t n);//n<16
    q1516_t sqrt1516(q1516_t x);
    q1516_t exp1516(q1516_t x);
    
    q1516_t sin1516(q1516_t);
    q1516_t cos1516(q1516_t);
#endif 