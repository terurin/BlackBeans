#pragma once
#ifndef __CLIP_HEADER_GUARD__
#define __CLIP_HEADER_GUARD__
    #include <stdbool.h>
    #include <stdint.h>
    #include "dsptype.h"
    
    static inline int16_t clip16(int32_t x){
        if (x>INT16_MAX)return INT16_MAX;
        if (INT16_MIN>x)return INT16_MIN;
        return x;
    }
    static inline int32_t clip32(int64_t x){
        if (x>INT32_MAX)return INT32_MAX;
        if (INT32_MIN>x)return INT32_MIN;
        return x;
    }
    
    static inline int32_t clip32_add(int32_t x,int32_t y){
        int64_t add = (int64_t)x+(int64_t)y;
        if (add>INT32_MAX)return INT32_MAX;
        if (INT32_MIN>add)return INT32_MIN;
        return add;
    }

    static inline int32_t clip32_sub(int32_t x,int32_t y){
        int64_t sub = (int64_t)x-(int64_t)y;
        if (sub>INT32_MAX)return INT32_MAX;
        if (INT32_MIN>sub)return INT32_MIN;
        return sub;
    }

    static inline int32_t clip32_range(int32_t x,int32_t max,int32_t min){
        if (x>max)return max;
        if(min<x)return min;
        return x;
    }
    
    static inline int32_t clip32_abs(int32_t x){
        //[2^31,2^31-1]->[0,2^31-1]
        if (x==INT32_MIN)return INT32_MAX;
        if (x<0)return -x;
        return x;
    }
    //x-> [-2*pi,2*pi]
    q1516_t clip1516_rad(q1516_t x);
    q1516_t clip1516_pi4(q1516_t x);
#endif
