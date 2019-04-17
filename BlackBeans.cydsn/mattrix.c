#include "mattrix.h"
#include <assert.h>
#include <FreeRTOS.h>
#include <math.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#define SWAP(a, b) \
    {typeof(a) __tmp = (a); (a) = (b); (b) = __tmp;}
float** mattrix_float_new(size_t x,size_t y){
    float** mattrix=(float**)pvPortMalloc(sizeof(float*)*y);
    for (size_t i=0;i<y;i++){
        mattrix[i]=(float*)pvPortMalloc(sizeof(float)*x);
    }
    return mattrix;
}

void mattrix_float_delete(float*** mattrix,size_t x,size_t y){
    assert(mattrix);assert(x>0);
    if (!*mattrix)return;
    for (size_t i=0;i<y;i++){
        vPortFree(*mattrix[i]);
    }
    vPortFree(*mattrix);
    *mattrix=NULL;
}

int32_t** mattrix_int32_new(size_t x,size_t y){
    int32_t** mattrix=(int32_t**)pvPortMalloc(sizeof(int32_t*)*y);
    for (size_t i=0;i<y;i++){
        mattrix[i]=(int32_t*)pvPortMalloc(sizeof(int32_t)*x);
    }
    return mattrix;
}

void mattrix_int32_delete(int32_t*** mattrix,size_t x,size_t y){
    assert(mattrix);assert(x>0);
    if (!*mattrix)return;
    for (size_t i=0;i<y;i++){
        vPortFree(*mattrix[i]);
    }
    vPortFree(*mattrix);
    *mattrix=NULL;
}

float** mattrix_float_fill(float** dest,float value,size_t x,size_t y){
    assert(dest);
    for (size_t i=0;i<y;i++){
        for (size_t j=0;j<x;j++){
            dest[i][j]=value;
        }
    }
    return dest;
}

float** mattrix_float_unit(float** dest,float value,size_t x,size_t y){
    assert(dest);
    for (size_t i=0;i<y;i++){
        for (size_t j=0;j<x;j++){
            dest[i][j]=(i!=j)?0:value;
        }
    }
    return dest;
}

q1516_t** mattrix_convert_float_to_q1516(q1516_t **dest,float** inst,size_t x,size_t y){
    assert(dest);
    assert(inst);
    for (size_t j=0;j<y;j++){
        for (size_t i=0;i<x;i++){
            dest[j][i]=inst[j][i]*(1<<16);
        }
    }
    return dest;
}


q1516_t* mattrix_dot_vector(q1516_t * d,
    const q1516_t** m,const q1516_t *v,size_t x,size_t y){
    assert(d);
    assert(m);
    assert(v);
    for (size_t i=0;i<y;i++){
        q3132_t sum=0;
        for (size_t j=0;j<x;j++){
            sum+=(q3132_t)m[i][j]*v[j];
        }
        d[i]=sum;
    }
    return d;
}

float** mattrix_float_inverse(float** dest,float** inst,size_t n){
    assert(dest);
    assert(inst);
    
    mattrix_float_unit(dest,1,n,n);
    //掃き出し法
    for(size_t i=0;i<n;i++){
        //ピボット
        float abs=fabsf(inst[i][i]);
        size_t max_idx=i;
        for (size_t j=i+1;j<n;j++){
            float abs_new=fabsf(inst[j][i]);
            if (abs<abs_new){
                abs=abs_new;
                max_idx=j;
            }
        }
        SWAP(inst[i],inst[max_idx]);
        SWAP(dest[i],dest[max_idx]);
        //inst[i][i]を1にする
        float s=inst[i][i];
        for (size_t j=i;j<n;j++)inst[i][j]/=s;
        for (size_t j=0;j<n;j++)dest[i][j]/=s;
        //非対角要素0を0にする
        for (size_t j=0;j<n;j++){
            if (i==j)continue;
            float tmp=inst[j][i];
            for (size_t k=0;k<n;k++){
                inst[j][k]-=inst[i][k]*tmp;
            }
        }
    }
    return dest;
}

char* mattrix_float_dump(char* result,size_t result_size,const float** mattrix,size_t x,size_t y){
    assert(result);
    assert(mattrix);
    char tmp[16];
    size_t i,j;
    snprintf(result,result_size,"mattrix[%dx%d]\n",x,y);
    for (i=0;i<y;i++){
        for(j=0;j<x-1;j++){
            snprintf(tmp,sizeof(tmp),"%4f,",mattrix[i][j]);
            strncat(result,tmp,result_size);
        }
        snprintf(tmp,sizeof(tmp),"%4f\n",mattrix[i][j]);
        strncat(result,tmp,result_size);
    }
    return result;
}


