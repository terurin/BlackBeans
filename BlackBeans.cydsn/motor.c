#include "motor.h"
#include "bldc.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdlib.h>
#include "clip.h"
#include "shell.h"
#include "text.h"
#define M_PI 3.141592
#include <math.h>
#define BUFFER_SIZE (256)

#define TABLE_SIZE (3)
static q1516_t table_x[TABLE_SIZE];
static q1516_t table_y[TABLE_SIZE];
static q1516_t table_theta[TABLE_SIZE];
static q1516_t table_raw_x[TABLE_SIZE];
static q1516_t table_raw_y[TABLE_SIZE];
static q1516_t table_raw_theta[TABLE_SIZE];
static q1516_t table_k[TABLE_SIZE];
//PID制御に関するパラメータ
static int pid_targets[TABLE_SIZE];//[pulse/sec]
static q1516_t pid_ps[TABLE_SIZE];
static q1516_t pid_is[TABLE_SIZE];
static q1516_t pid_ds[TABLE_SIZE];
//制御周期に関するパラメータ
static const int taps=260;//100Hz
static const int frequency=120;//Hz

static inline int clamp(int value,int max,int min){
    if (max<value){
        return max;
    }else if (value<min){
        return min;
    }else{
        return value;
    }
}


void motor_init(){
    //行列に関するパラメータ
    const static float angles[TABLE_SIZE]={0.0f,M_PI*2.0f/3.0f,-M_PI*2.0f/3.0f};//取付角度
    const static float lenghts[TABLE_SIZE]={0.08f,0.08f,0.08f};//取り付け位置[m]
    //pulseから長さに換算するパラメータ
    const static float wheels[TABLE_SIZE]={0.056f,0.056f,0.056f};//ホイールの直径[m]
    const static float gear[TABLE_SIZE]={2.0f,2.0f,2.0f};//ギア比
    const static float pulses[TABLE_SIZE]={48.0f,48.0f,48.0f};//一周あたりのパルス[pulse]
    
    unsigned int i;
    //q1516変換係数
    const static int gain =0xffff; 
    //PID係数変換用
    const static float ps[TABLE_SIZE]={4.0f,6.0f,6.0f};
    const static float is[TABLE_SIZE]={3.0f,3.0f,3.0f};
    const static float ds[TABLE_SIZE]={1.0f,1.0f,1.0f};
    
    for (i=0;i<TABLE_SIZE;i++){
        const float k=(2.0*pulses[i])/(wheels[i]*gear[i]);//変換係数
        table_x[i]=k*cosf(angles[i])*gain;
        table_y[i]=k*sinf(angles[i])*gain;
        table_theta[i]=k*lenghts[i]*gain;
        table_raw_x[i]=cosf(angles[i])*gain;
        table_raw_y[i]=sinf(angles[i])*gain;
        table_raw_theta[i]=lenghts[i]*gain;
        table_k[i]=k;
        pid_ps[i]=ps[i]*gain;
        pid_is[i]=is[i]*gain;
        pid_ds[i]=ds[i]*gain;
    }
    
}

void motor_fraction(int argc,char** argv){
    int i;
    for (i=1;i<argc;i++){
        bldc_control(i,NULL,NULL);
        bldc_write_raw(i-1,atoi(argv[i]));
    }
    for (;i<4;i++){
        bldc_control(i,NULL,NULL);
        bldc_write_raw(i-1,0);
    }
}

void motor_q15(int argc,char** argv){
    int i;
    for (i=1;i<argc;i++){
        bldc_control(i,NULL,NULL);
        bldc_write(i-1,atoi(argv[i]));
    }
    for (;i<bldc_count+1;i++){
        bldc_control(i,NULL,NULL);
        bldc_write(i-1,0);
    }
}

void motor_encoder(int argc,char** argv){
    (void)argc;
    (void)argv;
    char line[256];
    snprintf(line,sizeof(line),"%d %d %d\n",
        bldc_read(0),bldc_read(1),bldc_read(2));
    
    shell_puts(line);
}

void motor_encoder_clear(int argc,char** argv){
    (void)argc;
    (void)argv;
    for (int i=0;i<bldc_count;i++){
        bldc_clear(i);
    }
    shell_puts("encoder cleared\n");
}


void motor_status(int argc,char** argv){
    (void)argc;
    (void)argv; 
    int i;
    char buffer[BUFFER_SIZE]="";
    char tmp[BUFFER_SIZE];
    for (i=0;i<bldc_count-1;i++){
        sprintf(tmp,"%02x ",bldc_status(i)&0x7);
        strcat(buffer,tmp);
    }
    sprintf(tmp,"%02x\n",bldc_status(i)&0x7);
    strcat(buffer,tmp);
    shell_puts(buffer);
}


void motor_fraction_rocate(int argc,char** argv){
    //q1516変換用定数
    const static q1516_t gain = 0xffff;
    const q1516_t x = argc>1?atoi(argv[1])*gain:0;
    const q1516_t y = argc>2?atoi(argv[2])*gain:0;
    const q1516_t theta = argc>3?atoff(argv[3])*gain:0;
    int i;
    q3132_t sum;
    q1516_t sat;
    for (i=0;i<bldc_count;i++){
        sum=(q3132_t)table_raw_x[i]*x;
        sum+=(q3132_t)table_raw_y[i]*y;
        sum+=(q3132_t)table_raw_theta[i]*theta;
        sat = sum >>32;//duty
        sat=clamp(sat,pwm_period,-pwm_period);
        bldc_control(i,NULL,NULL);
        bldc_write_raw(i,sat);
    }
}

#include <UART.h>
static q1516_t motor_pid_0(void *context){
    (void)context;
    const static unsigned id=0;
    static int wait=0;
    static q1516_t result=0;
    static q1516_t last_error=0;
    static q1516_t sum_error=0;
    if (wait++==taps){
        wait=0;
        int now = bldc_read(id)*frequency;
        bldc_clear(id);
        int target =pid_targets[id];
        int error=target-now;
        q4716_t sum=0;
        sum+= (q4716_t)pid_ps[id]*error;
        sum+=(q4716_t)pid_is[id]*sum_error;
        sum+=(q4716_t)pid_ds[id]*(error-last_error);
        //update
        sum_error=clip32_add(error,sum_error);
        last_error=error;
        result=sum>>16;
    }
    return result;
}

static q1516_t motor_pid_1(void *context){
    (void)context;
    const static unsigned id=1;
    static int wait=0;
    static q1516_t result=0;
    static q1516_t last_error=0;
    static q1516_t sum_error=0;
    if (wait++==taps){
        wait=0;
        int now = bldc_read(id)*frequency;
        bldc_clear(id);
        int target =pid_targets[id];
        int error=target-now;
        q4716_t sum=0;
        sum+= (q4716_t)pid_ps[id]*error;
        sum+=(q4716_t)pid_is[id]*sum_error;
        sum+=(q4716_t)pid_ds[id]*(error-last_error);
        //update
        sum_error=clip32_add(error,sum_error);
        last_error=error;
        result=sum>>16;
    }
    return result;

}

static q1516_t motor_pid_2(void *context){
    (void)context;
    const static unsigned id=2;
    static int wait=0;
    static q1516_t result=0;
    static q1516_t last_error=0;
    static q1516_t sum_error=0;
    if (wait++==taps){
        wait=0;
        int now = bldc_read(id)*frequency;
        bldc_clear(id);
        int target =pid_targets[id];
        int error=target-now;
        q4716_t sum=0;
        sum+= (q4716_t)pid_ps[id]*error;
        sum+=(q4716_t)pid_is[id]*sum_error;
        sum+=(q4716_t)pid_ds[id]*(error-last_error);
        //update
        sum_error=clip32_add(error,sum_error);
        last_error=error;
        result=sum>>16;
    }
    return result;
}

void motor_pulse(int argc,char** argv){ 
    const static control_func controls[TABLE_SIZE]={motor_pid_0,motor_pid_1,motor_pid_2};
    int i;
    for (i=0;i<TABLE_SIZE;i++){
        pid_targets[i]=(argc>i+1)?atoi(argv[i+1]):0.0;
        bldc_clear(i);
        bldc_control(i,controls[i],NULL);
    }
}

void motor_move(int argc,char** argv){
    
}