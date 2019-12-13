#include "clip.h"
#include <math.h>

#define M_PI 3.14159265358979323846

q1516_t clip1516_rad(q1516_t x){
    const static q1516_t m2pi = 2*M_PI*(1<<16);  
    return x%m2pi;
}

q1516_t clip1516_pi4(q1516_t x){
    const static q1516_t m1pi2 = M_PI/2*(1<<16);  
    q1516_t tmp=x%m1pi2;
    if (tmp<0){
        return tmp+m1pi2;
    }
    return tmp;
}