#pragma once
#ifndef __DSP_TYPE_HEADER_GURAD__
#define __DSP_TYPE_HEADER_GUARD__
    #include<stdint.h>
    typedef int32_t q1516_t;
    typedef int64_t q3132_t;
    typedef int32_t q31_t;
    
    #define Q1516_CAST(X) (X*(1<<16))
#endif
    