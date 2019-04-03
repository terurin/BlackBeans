#include "dspmath.h"
#include <stddef.h>
#include <stdbool.h>
#include <assert.h>
#include <math.h>
#include "clip.h"

const q1516_t q1516_pi=Q1516_CAST(M_PI);
const q1516_t q1516_2pi=Q1516_CAST(M_PI*2);


uint32_t fact(uint32_t n){
    static int32_t memo[16]={1,1,-1};
    assert(n<sizeof(memo)/sizeof(memo[0]));
    //計算済み
    if (memo[n]>0)return memo[n];
    //未計算
    return memo[n]=fact(n-1)*n;
}

q1516_t sqrt1516(q1516_t x) {
    // newton方による解法
    const q1516_t eps = 1 << 2;
    const size_t limit = 8;  //試行回数の制限
    size_t cnt;
    q1516_t now = 1 << 16, diff;
    q3132_t f;
    q1516_t g;
    for (cnt = 0; cnt < limit; cnt++) {
        f = (((q3132_t)now * now) >> 16) - x;
        g = 2 * now;
        now -= diff = ((f << 16) / g);
        if (abs32(diff) < eps) {
            return now;
        }
    }
    return now;
}

q1516_t exp1516(q1516_t x) {
    //テイラー展開
    const size_t limit = 12;  //試行制限
    const q3132_t eps = 1 << 16;
    size_t cnt;
    int32_t fact = 1;
    //第一項と第二項の計算
    q3132_t pow = (q3132_t)x << 16;
    q3132_t now = pow + (1ll << 32), diff;
    //第n項の計算
    for (cnt = 2; cnt < limit; cnt++) {
        fact *= cnt;
        pow = (pow * x) >> 16;
        now += diff = pow / fact;
        if (abs64(diff) < eps) {
            return now >> 16;
        }
    }
    return now >> 16;
}



q1516_t sin1516(q1516_t x){
    x=clip1516_rad(x);
    const size_t limit =12;
    const q1516_t eps=1<<3;
    const q1516_t x2=(int64_t)x*x>>16;
    q1516_t xx=x<<16;
    q1516_t sum=xx,diff;
    for (size_t i=3;i<limit;i+=2){
        xx=(xx*x2)>>16;
        sum += diff=(i&0b10)?-xx/fact(i):xx/fact(i);   
        if (abs32(diff)<eps){
            return sum;
        }
    }
    return sum;
}

q1516_t cos1516(q1516_t x){
    x=clip1516_rad(x);
    const size_t limit =12;
    const q1516_t eps=1<<3;
    const q1516_t x2=(int64_t)x*x>>16;
    q1516_t xx=x<<16;
    q1516_t sum=1<<16,diff;
    for (size_t i=2;i<limit;i+=2){
        xx=(xx*x2)>>16;
        sum += diff=(i&0b10)?-xx/fact(i):xx/fact(i);   
        if (abs32(diff)<eps){
            return sum;
        }
    }
    return sum;
}