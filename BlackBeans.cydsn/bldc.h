#pragma once
#ifndef __BLDC_HEADER_GUARD__
#define __BLDC_HEADER_GUARD__
#include "dsptype.h"

    void bldc_init();
    
    void bldc_write_raw(int id,int16_t);
    void bldc_write(int id,q15_t);
    int bldc_read(int id);
    typedef q15_t(*control_func)(void*);
    void bldc_control(int id,control_func,void* context);
#endif
    