
#include "Encoder.h"

EncoderData_t EncoderData[2];       //编码器值，0-当前，1-前一次
int EncLeft = 0,EncRight = 0;       //左右轮编码器增量
float DisLeft = 0 , DisRight = 0;   //左右轮位移


void Disdance_TwoWheel()
{
	GetEncoder();   //先更新编码器值
	
	//得到左右编码器的方向
	int flag_left  = __HAL_TIM_IS_TIM_COUNTING_DOWN(&htim5);
	int flag_right = __HAL_TIM_IS_TIM_COUNTING_DOWN(&htim3);
	
	//若为正方向则距离加上增量，若为反方向则距离减去增量
	//左轮
	if(flag_left == 0)
	{
		EncLeft += (EncoderData[0].left - EncoderData[1].left);
	}
	if(flag_left == 1)
	{
		EncLeft -= (EncoderData[0].left - EncoderData[1].left);
	}
	//右轮
	if(flag_right == 0)
	{
		EncRight += (EncoderData[0].right - EncoderData[1].right);
	}
	if(flag_right == 1)
	{
		EncRight -= (EncoderData[0].right - EncoderData[1].right);
	}
	
	//将编码器增量转化为位移
	DisLeft  = EncLeft * Enc2Dis;
	DisRight = EncRight * Enc2Dis;
}

void GetEncoder()
{
	//更新前一次的数据
	EncoderData[1] = EncoderData[0];
	//获取当前编码器值
  EncoderData[0].left = __HAL_TIM_GET_COUNTER(&htim5);
  EncoderData[0].right = __HAL_TIM_GET_COUNTER(&htim3);
}
