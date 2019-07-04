#include "bldc.h"
#include <stdbool.h>
#include "project.h"
//定数定義
static const uint8_t ctrl_mask_none=0;
static const uint8_t ctrl_mask_dir=1<<0;
static const uint8_t ctrl_mask_kill=1<<1;
static const uint8_t ctrl_mask_reset=1<<2;
static const uint8_t status_mask_hall=0b111;
static const uint8_t status_mask_encoder=0b11<<3;
static const uint8_t status_mask_encoder_x=1<<3;
static const uint8_t status_mask_encoder_y=1<<4;
static const uint8_t status_mask_error=1<<5;
static const uint16_t pwm_period=7799;//10kHz@Bus=26MHz
static const uint8_t pwm_priority=1;//1/7 2番目の優先度
static const unsigned int motor_count=3;

static inline void ctrl_write_all(uint8_t value){
    BLDC1_Control_Write(value);
    BLDC2_Control_Write(value);
    BLDC3_Control_Write(value); 
}
//割り込み
static void bldc_event_0();
static void bldc_event_1();
static void bldc_event_2();
static void *event_context[3];
static control_func event_controls[3];

static inline void pwm_init(){
    //全て停止させる
    BLDC1_PWM_Stop();
    BLDC2_PWM_Stop();
    BLDC3_PWM_Stop();
    //周期設定
    BLDC1_PWM_WritePeriod(pwm_period);
    BLDC2_PWM_WritePeriod(pwm_period);
    BLDC3_PWM_WritePeriod(pwm_period);
    //波形設定(duty=0)
    BLDC1_PWM_WriteCompare(0);
    BLDC2_PWM_WriteCompare(0);
    BLDC3_PWM_WriteCompare(0);
    //割り込み設定
    BLDC1_PWMEvent_StartEx(bldc_event_0);
    BLDC2_PWMEvent_StartEx(bldc_event_1);
    BLDC3_PWMEvent_StartEx(bldc_event_2);
    BLDC1_PWMEvent_SetPriority(pwm_priority);//startExの後
    BLDC2_PWMEvent_SetPriority(pwm_priority);
    BLDC3_PWMEvent_SetPriority(pwm_priority);
    //再開*/
    BLDC1_PWM_Start();
    BLDC2_PWM_Start();
    BLDC3_PWM_Start();
}

static int diff[3];
static void hall_event_0(){
    diff[0]++;
}
static void hall_event_1(){
    diff[1]++;
}
static void hall_event_2(){
    diff[2]++;
}

static inline void hall_init(){
    BLDC1_HallEvent_StartEx(hall_event_0);
    BLDC2_HallEvent_StartEx(hall_event_1);
    BLDC3_HallEvent_StartEx(hall_event_2);
}

void bldc_init(){
    ctrl_write_all(ctrl_mask_reset);   
    ctrl_write_all(ctrl_mask_none);
    pwm_init();
    hall_init();
}

static inline int16_t abs16(int16_t x){
    if (x>0)return x;
    return -x;
}

void bldc_write_raw(int id,int16_t value){
    switch (id){
    case 0:
        BLDC1_Control_Write(value<0?ctrl_mask_dir:ctrl_mask_none);
        BLDC1_PWM_WriteCompare(abs16(value));
        return;
    case 1:
        BLDC2_Control_Write(value<0?ctrl_mask_dir:ctrl_mask_none);
        BLDC2_PWM_WriteCompare(abs16(value));
        return;
    case 2:
        BLDC3_Control_Write(value<0?ctrl_mask_dir:ctrl_mask_none);
        BLDC3_PWM_WriteCompare(abs16(value));
        return ;
    default:
        return;
    }
}

void bldc_write(int id, q15_t duty){
    //cast
    int16_t value = ((int32_t)pwm_period*duty)>>16;
    bldc_write_raw(id,duty);
}

int bldc_read(int id){
    return id<3?diff[id]:0;
}


static void bldc_event_0(){
    static const int id=0;
    const control_func func = event_controls[id];
    if (func){
        bldc_write(id,func(event_context[id]));
    }
    
}
static void bldc_event_1(){
    static const int id=1;
    const control_func func = event_controls[id];
    if (func){
        bldc_write(id,func(event_context[id]));
    }
}
static void bldc_event_2(){
    static const int id=2;
    const control_func func = event_controls[id];
    if (func){
        bldc_write(id,func(event_context[id]));
    }
}