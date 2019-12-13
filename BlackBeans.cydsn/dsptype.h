#pragma once
#ifndef __DSP_TYPE_HEADER_GURAD__
#define __DSP_TYPE_HEADER_GUARD__
    #include<stdint.h>
    
    typedef uint_fast32_t q1616_t;
    typedef int_fast32_t q0724_t;
    typedef int_fast32_t q1516_t;
    typedef int_fast64_t q4716_t;
    typedef int_fast64_t q3132_t;
    typedef int_fast64_t q1548_t;
    typedef int_fast32_t q31_t;
   
    #define Q1516_CAST(X) ((X)*(1<<16))
    
    typedef int_fast16_t q15_t;
    typedef uint_fast16_t q16_t;
#endif
    