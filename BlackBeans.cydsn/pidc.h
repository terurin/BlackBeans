#pragma once
#ifndef __PIDC_HEADER_GUARD__
#define __PICD_HEADER_GUARD__
    #include <stdbool.h>
    #include "dsptype.h"
    #include <stddef.h>
    struct pid_controllor{
        q1516_t p,i,d;
        q1516_t error_sum,error_last;
    };
    typedef struct pid_controllor pidc_t;
    static inline q1516_t pidc_sum(const pidc_t* self){
        return self->error_sum;
    }
    
    void pidc_init(pidc_t* self,float p,float i,float d);
    void pidc_clean(pidc_t*);
    q15_t pidc_control(pidc_t* self,q1516_t target,q1516_t feedback);
    char* pidc_watch(char* buffer,size_t size,const pidc_t*);
    
    struct priscaler{
        uint32_t priscale;
        uint32_t counter;
    };
    typedef struct priscaler priscaler_t;
    void priscaler_init(priscaler_t*,uint32_t priscale);
    void priscaler_clear(priscaler_t*);
    bool priscaler_count(priscaler_t*);
    
#endif
    