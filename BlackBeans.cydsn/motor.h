#pragma once
#ifndef __MOTOR_HEADER_GUARD__
#define __MOTOR_HEADER_GUARD__
    
void motor_init();
void motor_fraction(int argc,char** argv);
void motor_q15(int argc,char** argv);
void motor_encoder(int argc,char** argv);
void motor_encoder_clear(int argc,char** argv);
void motor_status(int argc,char** argv);
void motor_fraction_rocate(int argc,char** argv);
void motor_pulse(int argc,char** argv);
void motor_move(int argc,char** argv);
void motor_watch(int argc,char* argv);
#endif
