
#include "Delay.h"

static uint8_t  fac_us=0;//us延时倍乘数
static uint16_t fac_ms=0;//ms延时倍乘数

/**
  * @brief  初始化延迟函数
  *         
  * @param  系统时钟SYSCLK的值
  *         
  * @param  预分频值
  *
  * @retval None
  */
void delay_init(uint8_t SYSCLK)
{
	SysTick->CTRL&=0xfffffffb;//
	fac_us=SYSCLK/8;		    
	fac_ms=(uint16_t)fac_us*1000;
}								    

/**
  * @brief  毫秒延迟，由于SysTick->LOAD寄存器是24位的，所以nms<=16777215*8*1000/SYSCLK
*           如SYSCLK=168MHz，则最大延时为798ms
  * @param  延时毫秒数
  *         
  * @retval None
  */
void delay_ms(uint16_t nms)
{	 		  	  
	uint32_t temp;		   
	SysTick->LOAD=(uint32_t)nms*fac_ms;
	SysTick->VAL =0x00;         
	SysTick->CTRL=0x01 ;           
	do
	{
		temp=SysTick->CTRL;
	}
	while(temp&0x01&&!(temp&(1<<16)));
	SysTick->CTRL=0x00;       
	SysTick->VAL =0X00;         	    
}   


/**
  * @brief  微秒延迟，最大延时798us
  *        
  * @param  延时微秒数
  *         
  * @retval None
  */	    								   
void delay_us(uint32_t nus)
{		
	uint32_t temp;	    	 
	SysTick->LOAD=nus*fac_us;   		 
	SysTick->VAL=0x00;        
	SysTick->CTRL=0x01;     	 
	do
	{
		temp=SysTick->CTRL;
	}
	while(temp&0x01&&!(temp&(1<<16))); 
	SysTick->CTRL=0x00;       
	SysTick->VAL =0X00;      	 
}
