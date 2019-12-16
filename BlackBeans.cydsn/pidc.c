#include "pidc.h"
#include <assert.h>
#include "clip.h"
#include <string.h>
static inline q1516_t to_q1516(float x){
    return x*0xffff;   
}

void pidc_init(pidc_t* self,float p,float i,float d){
    assert(self);
    self->p=to_q1516(p);
    self->i=to_q1516(i);
    self->d=to_q1516(d);
    self->error_sum=0;
    self->error_last=0;   
}

void pidc_clean(pidc_t* self){
    assert(self);
    self->error_sum=0;
    self->error_last=0;   
}



//target,feedbackは同じ単位にすること
q15_t pidc_control(pidc_t* self,q1516_t target,q1516_t feedback){
    assert(self);
    const q1516_t error     = target-feedback;
    const q1516_t error_sum = clip32(error+self->error_sum);;
    const q1516_t error_diff= clip32(error-self->error_last);
        
    q3132_t sum=0;
    sum += (q3132_t)self->p*error;
    sum += (q3132_t)self->i*error_sum;
    sum += (q3132_t)self->d*error_diff;
    const q15_t result = clip16(sum>>32);
    //update
    self->error_last=error;
    self->error_sum=error_sum;
    return result;
}

char* pidc_watch(char* buffer,size_t size,const pidc_t* self){
    assert(self&&buffer);
    snprintf(buffer,size,"error=%d,sum=%d",self->error_last,self->error_sum);
    return buffer;
}

void priscaler_init(priscaler_t* self,uint32_t priscale){
    assert(self&&priscale);
    self->counter=0;
    self->priscale=priscale;
}

void priscaler_clear(priscaler_t* self){
    assert(self);
    self->counter=0;
}    

bool priscaler_count(priscaler_t* self){
    assert(self);
    if (++self->counter==self->priscale){
        self->counter=0;
        return true;
    }
    return false;
}
    