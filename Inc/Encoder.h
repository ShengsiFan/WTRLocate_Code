
#ifndef ENCODER_H
#define ENCODER_H

#include "tim.h"

#define Pi          3.1415926
#define Enc2Dis     50.8*Pi/4096        //一个脉冲对应的距离

typedef struct
{
	unsigned int left;   
	unsigned int right;   
}EncoderData_t;

void GetEncoder(void);
void Disdance_TwoWheel(void);

extern float DisLeft,DisRight;

#endif
