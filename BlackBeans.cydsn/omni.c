#include "omni.h"
#include "bldc.h"
#include <stddef.h>
#include <math.h>

typedef struct omni_table{
    q1516_t sin,cos,rad;
}omni_table_t;
//エクセルを参考してね
static const omni_table_t table[]={
    {0,         44938971,   22588790},
    {38918290,	-22469486,	22588790},
    {-38918291,	-22469486,	22588790}
};

static inline q15_t clamp(q3132_t x){
    const q4716_t y=x>>16;
    if (y>0x7fff)return 0x7fff;
    if (-0xffff>y)return -0x7fff;
    return y;
}

void omni_write(q1516_t vx,q1516_t vy,q1516_t rad){
    q15_t duty[3];
    q3132_t acc;
    unsigned int i;
    for (unsigned int i=0;i<3;i++){
        acc  =(q3132_t)vx*table[i].sin;
        acc +=(q3132_t)vy*table[i].cos;
        acc +=(q3132_t)rad*table[i].rad;
        duty[i]=clamp(acc);
    }
   
    for (unsigned int i=0;i<3;i++){
        bldc_write(i,duty[i]);   
    }
}