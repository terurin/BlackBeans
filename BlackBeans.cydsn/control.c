#include "control.h"
#include <assert.h>
#include <string.h>
#include "clip.h"
#include "dspmath.h"
#include <stdlib.h>
void pidc_init(pidc_t* obj,q1516_t p,q1516_t i,q1516_t d){
    assert(obj);   
    //パラメータ初期化
    memset(obj,0,sizeof(pidc_t));
    obj->p=p;
    obj->i=i;
    obj->d=d;
}


q1516_t pidc_control(pidc_t* obj,q1516_t error){
    assert(obj); 
    //エラーの計算
    q1516_t error_sum=obj->error_sum=clip32_add(obj->error_sum,error);
    q1516_t error_diff=clip32_sub(error,obj->error_last);
    obj->error_last=error;
    
    q3132_t sum=0;
    sum+=obj->output_last;
    sum+=obj->p*error;
    sum+=obj->i*error_sum;
    sum+=obj->d*error_diff;
    return obj->output_last=clip32(sum);
}

