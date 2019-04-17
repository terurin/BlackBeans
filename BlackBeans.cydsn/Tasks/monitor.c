#include "monitor.h"
#include <task.h>

#include <project.h>
#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include <MotorMonitor.h>
#include <Driver/timer.h>
#define forever() for(;;)
//define field
static xSemaphoreHandle converted;
static uint32_t raws[TASK_MONITOR_CHANNEL_SIZE][TASK_MONITOR_RECORD_SIZE];
static xSemaphoreHandle updated;
static uint32_t results[TASK_MONITOR_CHANNEL_SIZE];
//define constant
const static q1516_t filter[TASK_MONITOR_CHANNEL_SIZE]=
    {1<<14,1<<14,1<<14,1<<14};//TODO ちゃんと計算する
const static float vref_v =2.048f;
const static size_t bits=12;//解像度    
static void task_monitor_isr();
void task_monitor_init(){
    //セマフォ作成
    vSemaphoreCreateBinary(converted);
    assert(converted);
    //Mutex作成
    updated=xSemaphoreCreateMutex();
    assert(updated);
    //メモリーを初期化
    memset(&raws,0,sizeof(raws));
    memset(&results,0,sizeof(results));
}

static void task_monitor_isr(){
    static portBASE_TYPE quick_give;
    quick_give=pdFALSE;
    xSemaphoreGiveFromISR(converted,&quick_give);
}

static void task_monitor_process();
void task_monitor_lanch(const char* name,size_t size,size_t priority){
    //ADC開始
    MotorMonitor_Start();
    //割り込み登録
    MotorMonitor_IRQ_StartEx(task_monitor_isr);
    //タスク登録
    xTaskCreate((void(*)(void*))task_monitor_process,name,size,NULL,priority,NULL);
}

static void task_monitor_process(){

    const static portTickType interval_tick=100/portTICK_PERIOD_US;
    portTickType last_tick=xTaskGetTickCount();
    size_t pos=0;
    profiling_timer_init();
    forever(){
        //Convert 50[us]
        MotorMonitor_StartConvert();
        xSemaphoreTake(converted,portMAX_DELAY);
        //Fetch&Update 10[us]
        xSemaphoreTake(updated,portMAX_DELAY);
        for (size_t ch=0;ch<TASK_MONITOR_CHANNEL_SIZE;ch++){
            raws[ch][pos]=MotorMonitor_GetResult16(ch);
            q4716_t sum=0;
            //Digital Filterを適応する
            for (size_t i=0,j=pos;i<TASK_MONITOR_RECORD_SIZE;i++){
                sum+=(q3132_t)filter[i]*raws[ch][j];
                j=(j!=0)?j-1:TASK_MONITOR_RECORD_SIZE-1;
            }
            //結果を保存する
            results[ch]=sum>>16;
        }
        xSemaphoreGive(updated);

        vTaskDelayUntil(&last_tick,interval_tick);
    }
}

uint32_t task_monitor_read(task_monitor_channel_t channel){
    xSemaphoreTake(converted,portMAX_DELAY);
    const uint32_t result=results[channel];
    xSemaphoreGive(updated);
    return result;
}

q1516_t task_monitor_read_vbat_v(){
    const static float hi_ohm=10e3,low_ohm=1e3;//分圧抵抗
    const static q3132_t gain =(vref_v*(hi_ohm+low_ohm)/(low_ohm))*(1<<(16-bits));
    const uint32_t result = task_monitor_read(TASK_MONITOR_CHANNEL_VBAT);
    return gain*result;
}

q1516_t task_monitor_mxc_a(task_monitor_channel_t ch){
    assert(ch!=TASK_MONITOR_CHANNEL_VBAT);
    const static float a_per_v=50;
    const static q3132_t gain =a_per_v*vref_v*(1<<(16-bits));
    const uint32_t result = task_monitor_read(ch);
    return gain*result;
}