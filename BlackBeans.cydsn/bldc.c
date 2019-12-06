#include "bldc.h"
#include <stdbool.h>
#include <string.h>
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
static const uint8_t hall_priority=2;//2/7 3番目の優先度
const  int bldc_count=3;
//hall sensor
#define HOLD_SIZE (100) //Hall Sensorの変化量の保持数

static inline void ctrl_write_all(uint8_t value){
    BLDC1_Control_Write(value);
    BLDC2_Control_Write(value);
    BLDC3_Control_Write(value); 
}
//割り込み
static void bldc_event_0();
static void bldc_event_1();
static void bldc_event_2();
static void hall_event_0();
static void hall_event_1();
static void hall_event_2();

//割り込み時の制御
static void *event_context[3];
static control_func event_controls[3];
static int32_t counters[3];
static uint8_t counter_init=127;

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
    //割り込みハンドラーを削除
    memset(event_context,0,sizeof(event_context));
    memset(event_controls,0,sizeof(event_controls));
    //再開*/
    BLDC1_PWM_Start();
    BLDC2_PWM_Start();
    BLDC3_PWM_Start();
}




static inline void hall_init(){
    //ひとまず止める
    BLDC1_Counter_Stop();
    BLDC2_Counter_Stop();
    BLDC3_Counter_Stop();
    //割り込み設定
    memset(counters,0,sizeof(counters));
    //再開
    BLDC1_Counter_Start();
    BLDC2_Counter_Start();
    BLDC3_Counter_Start();
    //設定
    BLDC1_Counter_WriteCounter(counter_init);
    BLDC2_Counter_WriteCounter(counter_init);
    BLDC3_Counter_WriteCounter(counter_init);
    
}

void bldc_init(){
    ctrl_write_all(ctrl_mask_reset);   
    ctrl_write_all(ctrl_mask_none);
    pwm_init();
    hall_init();
    bldc_write_raw(0,0);
    bldc_write_raw(1,0);
    bldc_write_raw(2,0);
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
    bldc_write_raw(id,value);
}

void bldc_ctrl_write(int id,uint8_t ctrl){
    switch (id){
    case 0:
        BLDC1_Control_Write(ctrl);
        return;
    case 1:
        BLDC2_Control_Write(ctrl);
        return ;
    case 2:
        BLDC3_Control_Write(ctrl);
        return ;
    }
}

int bldc_read(int id){
    return id<bldc_count?counters[id]:0;
}

uint32_t bldc_status(int id){
    //ビット列
    switch (id){
    case 0:
        return BLDC1_Status_Read();
    case 1:
        return BLDC2_Status_Read();
    case 2:
        return BLDC3_Status_Read();
    default:
        return 0;
    };
}

uint32_t bldc_timer(int id){
    switch (id){
    case 0:
        return BLDC1_PWM_ReadCounter();
    case 1:
        return BLDC2_PWM_ReadCounter();
    case 2:
        return BLDC3_PWM_ReadCounter();
    default:
        return 0;
    };  
}

void bldc_control(int id,control_func ctrl,void* context){
    if (id<bldc_count){
        event_context[id]=context;
        event_controls[id]=ctrl;
    }
    
}

static int position(unsigned int state){
    switch (state){
        case 0b001:return 0;
        case 0b011:return 1;
        case 0b010:return 2;
        case 0b110:return 3;
        case 0b100:return 4;
        case 0b101:return 5;
        default:return -1;//Error
    }
}
#define ABS(X) (X)>0?(X):-(X)
static int diff(int now,int last){
    if (now<0&&last<0)return 0;
    static const int table [6][6]={   
        {0,1,2,0,-2,-1},
        {-1,0,1,2,0,-2},
        {-2,-1,0,1,2,0},
        {0,-2,-1,0,-1,-2},
        {2,0,-2,-1,0,1},
        {1,2,0,-2,-1,0}};
        
    return table[last][now];
}

static void bldc_event_0(){
    static const int id=0;
    const control_func func = event_controls[id];
    static int last=0;
    static int count=0;
    static int hold=0;
    int now=bldc_status(id)&0x7;
    hold+=diff(position(now),position(last));
    last=now;
    
    count=(count+1)%26;
    if (count==25){
        counters[id]=hold;
        hold=0;
    }
    
    
    if (func){
        bldc_write(id,func(event_context[id]));
    }

}
static void bldc_event_1(){
    static const int id=1;
    const control_func func = event_controls[id];

    static int last=0;
    static int count=0;
    static int hold=0;
    int now=bldc_status(id)&0x7;
    hold+=diff(position(now),position(last));
    last=now;
    
    count=(count+1)%26;
    if (count==25){
        counters[id]=hold;
        hold=0;
    }
    
    if (func){
        bldc_write(id,func(event_context[id]));
    }
}

static void bldc_event_2(){
    static const int id=2;
    const control_func func = event_controls[id];
    static int last=0;
    static int count=0;
    static int hold=0;
    int now=bldc_status(id)&0x7;
    hold+=diff(position(now),position(last));
    last=now;
    
    count=(count+1)%26;
    if (count==25){
        counters[id]=hold;
        hold=0;
    }
    if (func){
        bldc_write(id,func(event_context[id]));
    }
}