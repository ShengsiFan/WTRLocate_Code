/**
  ******************************************************************************
  * 文件名          : ADS8328.c
  * 创建时间        : 2020.03.11
  * 作者            : 范声思
	* 最近修改        ：
  ******************************************************************************
  *                                  文件描述                                  
  ******************************************************************************
  *	功能：
	*      定义了ADS8328双通道ADC芯片的操作
	*
  ******************************************************************************
	*                                  版本信息                                  
  ******************************************************************************
	*v1.0            ：包含ADS8328默认初始化和自定义初始化，及默认配置下的操作                                                                            
	*	                                                                            
	*                                                                            
  ******************************************************************************
	*                                    说明                                    
  ******************************************************************************
  * 1.本代码基于STM32F405RGTx开发，编译环境为Keil 5。
  * 2.本代码只适用于WTRLocate全场定位模块，不建议用作其他用途
  * 3.本代码以UTF-8格式编码，请勿以ANSI编码形式打开
  * 4.本代码最终解释权归哈尔滨工业大学（深圳）南工问天战队所有
  *
  * Copyright (c) 2020 哈尔滨工业大学（深圳）南工问天战队 版权所有
  ******************************************************************************
  */
#include "ADS8328.h"

GyroData_t GyroData;

/**
  * @brief  获得陀螺仪数据
  *         
	* @Param  None
  * @retval None
  */
void GyroGetData()
{
	ADS8328_TempHandler();
	ADS8328_AngleHandler();

}


/**
  * @brief  温度解算。2.5V对应25摄氏度，每变化一度，电压变化9mV
  *         
	* @Param  None
  * @retval None
  */
void ADS8328_TempHandler()
{
	int RefData = ADS8328_Decoder_Tag_Temp();
	
  float Vol = 5 * RefData / 65535;   //实际电压
	GyroData.Temperature = 25 + (Vol - 2.5)/0.009;
}


/**
  * @brief  角度解算。2.5V对应0度，每变化一度，电压变化6mV
  *         
	* @Param  由原始数据转成的int
  * @retval None
  */
void ADS8328_AngleHandler()
{
	int RefData = ADS8328_Decoder_Tag_Temp();
	
  float Vol = 5 * RefData / 65535;   //实际电压
	GyroData.Temperature = (Vol - 2.5)/0.006;
}


/**
  * @brief  Tag模式解码温度
  *         
	* @Param  None
  * @retval 温度的ADC整型结果，最大34733，最小30802
  */
int ADS8328_Decoder_Tag_Temp()
{
	int ReceiveData,ReceiveTag;
	uint32_t OriginData;
	
	//只要收到的不是温度数据，就一直接收
	while(ReceiveTag != 1)
	{
		OriginData = ADS8328_Read_TagMode();
	  ReceiveTag  = (int)((OriginData & 0x0000ffff) >> 15);
	}
	
  //将数据存入ReceiveData中 
	ReceiveData = (int)(OriginData >> 16);
	//设置上下限
	if(ReceiveData > 34733)
	{
		ReceiveData = 34733;
	}
	if(ReceiveData < 30802)
	{
		ReceiveData = 30802;
	}
  return ReceiveData;
}


/**
  * @brief  Tag模式解码角度
  *         
	* @Param  None
  * @retval 角度的ADC整型结果，最大56360，最小9175
  */
int ADS8328_Decoder_Tag_Angle()
{
	int ReceiveData,ReceiveTag;
	uint32_t OriginData;
	
	//只要收到的不是角度数据，就一直接收
	while(ReceiveTag != 0)
	{
		OriginData = ADS8328_Read_TagMode();
	  ReceiveTag  = (int)((OriginData & 0x0000ffff) >> 15);
	}
	
	//将数据存入ReceiveData中 
	ReceiveData = (int)(OriginData >> 16);
	//设置上下限
	if(ReceiveData > 56360)
	{
		ReceiveData = 56360;
	}
	if(ReceiveData < 9175)
	{
		ReceiveData = 9175;
	}
  return ReceiveData;
}


/**
  * @brief  Tag模式读取转换结果
  *         
	* @Param  None
  * @retval 数据+Tag拼起来的32bit
  */
uint32_t ADS8328_Read_TagMode()
{
	uint16_t OriginData[2];   //接收数据数组
	uint32_t ReturnData = 0x00000000;      //返回值
	
	SPI_ADS_CONVST(0);   //开始转换
  delay_us(1); 
  SPI_ADS_CONVST(1);   //转换信号回高	
	SPI_ADS_CS2(0);      //片选
	HAL_SPI_Receive(&hspi2,(uint8_t*)OriginData,2,10);    //读数据
	SPI_ADS_CS2(1);      //片选信号回高
	
	//把收到的数组值存入ReturnData
	ReturnData += (OriginData[0] << 16);   
	ReturnData +=  OriginData[1];
	
	return ReturnData;
}


/**
  * @brief  初始化ADS8328芯片
  *         
  * @Param  None
  * @retval None
  */
void ADS8328_Init()
{
	uint16_t pData;
	
	/** @（DEFAULT）默认配置。若要默认设置，打开这一段代码，并注释（SET）段代码。
  * @{
  */
	//默认配置
	SPI_ADS_CS2(ADS_START );
	HAL_SPI_Transmit(&hspi2, (uint8_t*)ADS_DEFAULT_MODE, 1, 10);
	SPI_ADS_CS2(ADS_END );
	delay_us(100);
	
	//检验是否正确发送
	SPI_ADS_CS2(ADS_START );
	HAL_SPI_Transmit(&hspi2, (uint8_t*)0x0c, 1, 10);  //发送读取命令
	SPI_ADS_CS2(ADS_END );
	delay_us(100);
	
	SPI_ADS_CS2(ADS_START );
	HAL_SPI_Receive(&hspi2, (uint8_t*)&pData, 1, 10);
	SPI_ADS_CS2(ADS_END );
	if(pData != (0xeff << 4))
	{
		Error_Handler();	
	}
	/** @（DEFAULT）
  * @}
  */
	
	
	/** @（SET）自定义配置。若要自定义设置，打开这一段代码，并注释（DEFAULT）段代码。
  * @{
  */
//	//选择自定义模式
//	SPI_ADS_CS2(ADS_START );
//	HAL_SPI_Transmit(&hspi2, (uint8_t*)ADS_SET_MODE, 1, 10);
//	SPI_ADS_CS2(ADS_END );
//	delay_us(100);
//	//发送配置内容
//  SPI_ADS_CS2(ADS_START );
//	HAL_SPI_Transmit(&hspi2, (uint8_t*)（12位配置内容）, 1, 10);  //详细配置内容，请CIRL+F：ADS8328_SELFSET
//	SPI_ADS_CS2(ADS_END );
//	
//	//检验是否正确发送
//	SPI_ADS_CS2(ADS_START );
//	HAL_SPI_Transmit(&hspi2, (uint8_t*)0x0c, 1, 10);  //发送读取命令
//	SPI_ADS_CS2(ADS_END );
//	delay_us(100);
//	
//	SPI_ADS_CS2(ADS_START );
//	HAL_SPI_Receive(&hspi2, (uint8_t*)&pData, 1, 10);
//	SPI_ADS_CS2(ADS_END );
//	if(pData != (（12位配置内容） << 4))
//	{
//		Error_Handler();	
//	}
	/** @（SET）
  * @}
  */
}	

