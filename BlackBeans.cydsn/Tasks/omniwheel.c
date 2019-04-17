#include "omniwheel.h"
#include <assert.h>
#include <math.h>
#include <string.h>
#include <FreeRTOS.h>
#include <task.h>



void omniwheel_init(omniwheel_t* omni,uint32_t interval_us,
    wheel_layout_t* layouts,size_t size){
    assert(omni);
    assert(layouts);
    memset(omni,0,sizeof(omniwheel_t));    
    omni->interval_us=interval_us;
    omni->wheel_size=size;
    
    //メモリーを確保する
    float **matt=(float**)pvPortMalloc(sizeof(float*)*size);
    q1516_t **divide=(q1516_t**)pvPortMalloc(sizeof(q1516_t*)*size);
    //float ***invs=(float***)pvPortMalloc(sizeof(float**)*size-2);
    q1516_t ***merges=(q1516_t***)pvPortMalloc(sizeof(q1516_t**)*size-2);
   
    for(size_t i=0;i<size;i++){
        matt[i]=(float*)pvPortMalloc(sizeof(float)*3);
        divide[i]=(q1516_t*)pvPortMalloc(sizeof(q1516_t)*3);
    }
    
    for(size_t i=0;i<size-2;i++){
        //invs[i]=(float**)pvPortMalloc(sizeof(float*)*3);
        merges[i]=(q1516_t**)pvPortMalloc(sizeof(q1516_t*)*3);
        for (size_t j=0;j<3;j++){
            //invs[i][j]=(float*)pvPortMalloc(sizeof(float)*3);
            merges[i][j]=(q1516_t*)pvPortMalloc(sizeof(q1516_t)*3);            
        }
    }
    //ベクトル分解行列を計算する
    for(size_t i=0;i<size;i++){
        matt[i][0]=cosf(layouts[i].angle_rad);
        matt[i][1]=sinf(layouts[i].angle_rad);
        matt[i][2]=layouts[i].distance_m;
        for (size_t j=0;j<3;j++){
            divide[i][j]=matt[i][j]*(1<<16);
        }
    }
    
}
    
void omniwheel_assign(omniwheel_t* omni,
    void (*wheel_target_mps)(void*,q1516_t),
    q1516_t (*wheel_feedback_mps)(void*),
    void** wheel_objects,size_t size){
    assert(omni);
    assert(omni->wheel_size==size);
    
    omni->wheel_objects=wheel_objects;
    omni->wheel_target_mps=wheel_target_mps;
    omni->wheel_feedback_mps=wheel_feedback_mps;
}
    
void omniwheel_lanch(omniwheel_t* omni,const char*name,size_t stack,size_t priority){
    typedef void(*task_t)(void*);
    assert(omni);
    xTaskCreate((task_t)omniwheel_process,name,stack,omni,priority,NULL);
}

void omniwheel_process(omniwheel_t* omni){
    assert(omni);
    for(;;){
        
    } 
}