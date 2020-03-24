

#include "CanOperation.h"

/**
  * @brief  CAN通信初始化，包含过滤器设置，开启CAN，使能CAN中断
  * @param  CAN句柄
  * @retval HAL状态
  */
void CanInit(CAN_HandleTypeDef* canHandle)
{
	CAN_FilterTypeDef Filter;
	
	Filter.FilterActivation = CAN_FILTER_ENABLE;
	Filter.FilterBank = 0;
	Filter.FilterFIFOAssignment = CAN_FILTER_FIFO0;
	Filter.FilterIdHigh = 0x0000;
	Filter.FilterIdLow = 0x0000;
	Filter.FilterMaskIdHigh = 0x0000;
	Filter.FilterMaskIdLow = 0x0000;
	Filter.FilterMode = CAN_FILTERMODE_IDMASK;
	Filter.FilterScale = CAN_FILTERSCALE_32BIT;
	Filter.SlaveStartFilterBank = 14;
	
	HAL_CAN_ConfigFilter(canHandle,&Filter);
	
	if(HAL_CAN_Start(canHandle) != HAL_OK)
	{
		Error_Handler();
	}
	if(HAL_CAN_ActivateNotification(canHandle,CAN_IT_TX_MAILBOX_EMPTY) != HAL_OK)
	{
		Error_Handler();
	}
}


/**
  * @brief  发送结果
  * @param  CAN句柄
  * @retval HAL状态
  */
void ReportResult()
{
	CAN_TxHeaderTypeDef TxHeader;   //定义发送结构体变量
	uint8_t ReportData[4];
	
	RunDevices();   //运行各设备，更新数据
	Calculate();    //计算得到定位信息
	
	//配置发送结构体变量
	TxHeader.DLC = 0x04;
	TxHeader.IDE = CAN_ID_STD;
	TxHeader.RTR = CAN_RTR_DATA;
	TxHeader.StdId = 0x200 ;
	
	ReportData[0] = (uint8_t)Robot_X;
	ReportData[1] = (uint8_t)Robot_Y;
	ReportData[2] = (uint8_t)Robot_Angle;
	ReportData[3] = (uint8_t)GyroData.Temperature;
	
	if(HAL_CAN_AddTxMessage(&hcan1,&TxHeader,ReportData,CAN_FILTER_FIFO0) != HAL_OK)
	{
		Error_Handler();
	}
	
}
