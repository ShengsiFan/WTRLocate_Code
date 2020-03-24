
#include "InitAll.h"

/**
  * @brief  初始化所有外设
  * @retval None
  */
void InitAll()
{
	delay_init(168);  //初始化延时函数
	CanInit(&hcan1);  //初始化CAN
	ADS8328_Init();   //初始化ADS8328
	HAL_TIM_Encoder_Start(&htim3, TIM_CHANNEL_ALL);   //开启编码器1
	HAL_TIM_Encoder_Start(&htim5, TIM_CHANNEL_ALL);   //开启编码器2
	
}
