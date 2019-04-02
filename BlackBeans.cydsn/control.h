#pragma once
#ifndef __CONTROL_HEADER_GURAD__
#define __CONTROL_HEADER_GUARD__

    #include "dsptype.h"
    #include <stddef.h>
    
    struct pidc{
        q1516_t p,i,d;
        q1516_t output_last;
        q1516_t error_sum;
        q1516_t error_last;
    };
   
    typedef struct pidc pidc_t;
    
    void pidc_init(pidc_t* obj,q1516_t p,q1516_t i,q1516_t d);
    q1516_t pidc_control(pidc_t* obj,q1516_t error);
    q1516_t pidc_error(const pidc_t* obj);
    
    //タイヤの制御を行う
    //速度からモータへのduty比を計算する
    struct wheel{
        q1516_t gain;
    };
    typedef struct wheel wheel_t;
    
    void wheel_init(wheel_t*, q1516_t radius);
    q31_t wheel_control(wheel_t*, );
    
    
    
#endif
    