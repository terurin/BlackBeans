#pragma once
#ifndef __DRIVER_ADC_HEADER_GUARD__
#define __DIRVER_ADC_HEADER_GUARD__
    
#include <dsptype.h>
void adc_init();

enum adc_channel{
    ADC_CHANNEL_VBAT,
    ADC_CHANNEL_M1C,
    ADC_CHANNEL_M2C,
    ADC_CHANNEL_M3C
};
typedef enum adc_channel adc_channel_t;
inline int adc_read(adc_channel_t);
q1516_t adc_read_vbat_v();
q1516_t adc_read_m1_a();
q1516_t adc_read_m2_a();
q1516_t adc_read_m3_a();
    
#endif
    