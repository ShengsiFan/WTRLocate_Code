/**
  ******************************************************************************
  * 文件名          : ADS8328.h
  * 创建时间        : 2020.03.11
  * 作者            : 范声思
  ******************************************************************************
  *                                  文件描述                                  
  ******************************************************************************
  *	功能：
	*      定义了ADS8328双通道ADC芯片的操作
	*
  ******************************************************************************
	*                                    说明                                    *
  ******************************************************************************
  * 1.本代码基于STM32F405RGTx开发，编译环境为Keil 5。
  * 2.本代码只适用于WTRLocate全场定位模块，不建议用作其他用途
  * 3.本代码以UTF-8格式编码，请勿以ANSI编码形式打开
  * 4.本代码最终解释权归哈尔滨工业大学（深圳）南工问天战队所有
  *
  * Copyright (c) 2020 哈尔滨工业大学（深圳）南工问天战队 版权所有
  ******************************************************************************
  */
#ifndef ADS8328_H
#define ADS8328_H

#include "Delay.h"
#include "spi.h"


	
/** @defgroup 开始、结束信号
  * @{
  */
#define ADS_START        0   //开始
#define ADS_END          1   //结束
/**
  * @}
  */
          

/** @defgroup 配置模式选择，默认或自定义配置
  * @{
  */
#define ADS_DEFAULT_MODE      0x0f   //默认模式
#define ADS_SET_MODE          0x0e   //自定义模式
#define ADS_ISSUE             0x0c   //读取配置
/**
  * @}
  */

/** @defgroup ADS8328_SELFSET自定义配置所有选择（从上到下，由高位到低位）
  * @{
  */
/*
 *位数   配置内容       具体：0                                           1
 * ————————————————————————————————————————————————————————————————————————————————
 *   1   通道选择             手动选择两个中的另一个通道                  自动开启所有通道   
 *   2   转换时钟源设置       SCLK/2                                      内部时钟
 *   3   转换触发             在EOC电平抬高后4个内部时钟后自动开始        手动触发，在CONVST下降沿开始
 *   4   无内容，必须置0
 *   5   10脚有效电平         高电平有效                                  低电平有效
 *   6   10脚功能             中断INT                                     EOC
 *   7   10脚是否用串联模式   用串联式                                    用作EOC或INT模式
 *   8   是否启用自动休眠     启动自动休眠                                不启动自动休眠
 *   9   是否休眠             休眠                                        苏醒
 *  10   深度休眠             休眠                                        苏醒
 *  11   标签位               关闭标签位                                  标签位启用，且在第17个SCLK时钟出现
 *  12   RESET                RESET                                       正常运行
*/
/**
  * @}
  */
	
/** @defgroup 操作定义
  * @{
  */
#define SPI_ADS_CS2(status)	     HAL_GPIO_WritePin(GPIOB, GPIO_PIN_12, (GPIO_PinState)status)  //软件片选
#define SPI_ADS_EOC              HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_0)                           //转换结束通知
#define SPI_ADS_CONVST(status)   HAL_GPIO_WritePin(GPIOB, GPIO_PIN_1, (GPIO_PinState)status)   //转换起止信号，参数为开始、结束信号
/**
  * @}
  */
	
	
/**
  * @brief 陀螺仪数据结构体
  */
typedef struct
{
	float Temperature;   //温度
	float Angle;         //角度
}GyroData_t;



/**
  * @brief 函数原型  
  */
void ADS8328_Init(void);
void GyroGetData(void);
void ADS8328_TempHandler(void);
void ADS8328_AngleHandler(void);
int ADS8328_Decoder_Tag_Temp(void);
int ADS8328_Decoder_Tag_Angle(void);
uint32_t ADS8328_Read_TagMode(void);

extern GyroData_t GyroData;


#endif
