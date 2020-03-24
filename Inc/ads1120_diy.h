#ifndef __ADS1120_H
#define __ADS1120_H
#include "spi.h"

typedef struct 
{
	SPI_HandleTypeDef *hspi;
	GPIO_TypeDef *CSPort;
	GPIO_TypeDef *DRDYPort;
	uint16_t CSpin;
	uint16_t DRDYpin;
	//REG0
	uint8_t Channel;
	uint8_t Gain;
	uint8_t PGA_Bypass;
	//REG1
	uint8_t DataRate;
	uint8_t OperateMode;
	uint8_t ConversionMode;
	uint8_t TemperatureMode;
	uint8_t BurnoutMode;
	//REG2
	uint8_t RefSelect;
	uint8_t filter;
	uint8_t PSWswitch;
	uint8_t IDACcurrval;
	//REG3
	uint8_t IDAC1curr;
	uint8_t IDAC2curr;
	uint8_t DRDYMode;
	
	uint8_t status;
	uint16_t data;
	
}ADS1120_HandleTypeDef;
extern ADS1120_HandleTypeDef hads;

//Register 0~3
#define REG0	0x00
#define REG1	0x01
#define	REG2	0x02
#define REG3	0x03

//Commands
#define	RESET	0x06	//Reset the device
#define	START	0x08	//Start or restart conversions
#define POWERDOWN	0x02	//Enter power-down mode
#define	RDATA		0x10	//Read data by command
#define	RREG		0x20	//read register,upper 4 bits are fixed
#define	WREG		0x40	//write register,upper 4 bits are fixed

//REG0 Config
#define DIFF01		0x00	// AIN P = AIN0, AIN N = AIN1 (default)
#define DIFF02		0x10	// AIN P = AIN0, AIN N = AIN2
#define	DIFF03		0x20	// AIN P = AIN0, AIN N = AIN3
#define DIFF12		0x30	// AIN P = AIN1, AIN N = AIN2	
#define	DIFF13		0x40	// AIN P = AIN1, AIN N = AIN3
#define DIFF23		0x50	// AIN P = AIN2, AIN N = AIN3
#define DIFF10		0x60	// AIN P = AIN1, AIN N = AIN0
#define DIFF32		0x70	// AIN P = AIN3, AIN N = AIN2
#define AIN0		0x80	// AIN P = AIN0, AIN N = AVSS
#define AIN1		0x90	// AIN P = AIN1, AIN N = AVSS
#define AIN2		0xa0	// AIN P = AIN2, AIN N = AVSS
#define AIN3		0xb0	// AIN P = AIN3, AIN N = AVSS
#define REFBY4		0xc0	// (V (REFPx) ¨C V (REFNx) ) / 4 monitor (PGA bypassed)
#define AVDDBY4		0xd0	// (AVDD ¨C AVSS) / 4 monitor (PGA bypassed)
#define SHORTED		0xe0	// AIN P and AIN N shorted to (AVDD + AVSS) / 2

#define GAIN1	0x00	//Gain = 1 (default)
#define GAIN2	0x02	//Gain = 2
#define GAIN4	0x04
#define GAIN8	0x06
#define GAIN16	0x08
#define GAIN32	0x0a
#define GAIN64	0x0c
#define GAIN128	0x0e
#define PGA_ENABLE	0x00
#define	PGA_BYPASS	0x01

//REG1 Config
//Operatint Mode
#define NORMAL_MODE	0x00	//Normal mode (256-kHz modulator clock, default)
#define DUTYCYCLE_MODE	0x08	// Duty-cycle mode (internal duty cycle of 1:4)
#define	TURBO_MODE	0x10	//Turbo mode (512-kHz modulator clock)
//Data Rate		Only high rates listed
#define NORMAL_1000	0xc0
#define NORMAL_600	0xa0
#define	DUTYCYCLE_250	0xc0
#define DUTYCYCLE_150	0xa0
#define TURBO_2000	0xc0
#define	TURBO_1200	0xa0
//Conversion Mode
#define SINGLE_SHOT_MODE	0x00
#define CONTINUOUS_MODE		0x04
//Temperature Mode
#define	TEMPERATURE_ENABLE	0x02
#define	TEMPERATURE_DISABLE	0x00
//Burn-out current sources
#define BURN_CURRENT_ON		0x01
#define BURN_CURRENT_OFF	0x00

//REG2 Config
//Reference Voltage Select
#define INTERNAL_REF	0x00	// Internal 2.048-V reference selected (default)
#define	EXTERNAL_REF0	0x40	//External reference selected using dedicated REFP0 and REFN0 inputs
#define EXTERNAL_REF1	0x80	//External reference selected using AIN0/REFP1 and AIN3/REFN1 inputs
#define	SUPPLY_REF		0xc0	// Analog supply (AVDD ¨C AVSS) used as reference
//FIR filter	Only use these bits together with the 20-SPS setting in normal mode and the 5-SPS setting in duty-cycle mode. Set to 00 for all other data rates.
#define NO_REJECTION	0x00	// No 50-Hz or 60-Hz rejection (default)
#define REJECTION_50_60	0x10	// Simultaneous 50-Hz and 60-Hz rejection	
#define REJECTION_50HZ	0x20	//50-Hz rejection only
#define	REJECTION_60HZ	0x30	//60-Hz rejection only
//Low-side power switch configuration
#define PSW_OPEN	0x00	//Switch is always open (default)
#define PSW_CLOSE	0x08	// Switch automatically closes when the START/SYNC command is sent and opens when the POWERDOWN command is issued
//IDAC Currents
#define IDAC_OFF	0x00	//IDAC Current OFF
#define IDAC_50		0x02	//50 ¦ÌA
#define	IDAC_100	0x03
#define	IDAC_250	0x04
#define	IDAC_500	0x05
#define	IDAC_1000	0x06
#define	IDAC_1500	0x07


//REG3 Config
//IDAC1 routing
#define IDAC1_DISABLE	0x00
#define	IDAC1_AIN0	0x20
#define IDAC1_AIN1	0x40
#define	IDAC1_AIN2	0x60
#define	IDAC1_AIN3	0x80
#define	IDAC1_REFP0	0xa0
#define	IDAC1_REFN0	0xc0
//IDAC2 routing
#define IDAC2_DISABLE	0x00
#define	IDAC2_AIN0	0x04
#define IDAC2_AIN1	0x08
#define	IDAC2_AIN2	0x0c
#define	IDAC2_AIN3	0x10
#define	IDAC2_REFP0	0x14
#define	IDAC2_REFN0	0x18
//DRDY Mode
#define DRDY_1PIN	0x00	// Only the dedicated DRDY pin is used to indicate when data are ready (default)
#define	DRDY_2PIN	0x02	// Data ready is indicated simultaneously on DOUT/DRDY and DRDY


#define CS_0	HAL_GPIO_WritePin(hads.CSPort,hads.CSpin,GPIO_PIN_RESET)
#define CS_1	HAL_GPIO_WritePin(hads.CSPort,hads.CSpin,GPIO_PIN_SET)

void ADS_InitGPIO(void);
void ADS_Init(void);
void ADS_Config(void);
void ADS_Start(void);
void ADS_Powerdown(void);
void ADS_Reset(void);
void SPI_ReadByte(uint8_t *data);
uint8_t SPI_WriteByte(uint8_t data);
void ADS_ReadReg(uint8_t reg,uint8_t len,uint8_t *r_buf);
void ADS_WriteReg(uint8_t reg,uint8_t len,uint8_t *t_buf,uint8_t *r_buf);
uint16_t ADS_ReadData(void);
uint16_t ADS_ReadDataByCommand(void);
void ADS_SwitchChannel(void);
#endif

