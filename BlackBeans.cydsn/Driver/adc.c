#include "adc.h"
#include <project.h>

void adc_init(){
    AnalogMux_Start();
    ADC_SAR_Start();
}

inline int adc_read(adc_channel_t ch){
    AnalogMux_FastSelect(ch);
    ADC_SAR_StartConvert();
    ADC_SAR_IsEndConversion(ADC_SAR_WAIT_FOR_RESULT);
    return ADC_SAR_GetResult16();
}
q1516_t adc_read_vbat_v(){
    return adc_read(ADC_CHANNEL_VBAT);
}
q1516_t adc_read_m1_a(){
    return adc_read(ADC_CHANNEL_M1C);
}

q1516_t adc_read_m2_a(){
    return adc_read(ADC_CHANNEL_M2C);
}
q1516_t adc_read_m3_a(){
    return adc_read(ADC_CHANNEL_M3C);
}