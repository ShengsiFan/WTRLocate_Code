/*
 *    ADS1120.c
 *
 *    This code is designed to perform standard command and control
 *    operations on the ADS1120 over a SPI bus. Functions exist to setup,
 *    configure, and read conversion data from the ADS1120.
 *
 *
 * Copyright (C) 2018 Texas Instruments Incorporated - http://www.ti.com/
 *
 *
 *  Redistribution and use in source and binary forms, with or without
 *  modification, are permitted provided that the following conditions
 *  are met:
 *
 *    Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *
 *    Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the
 *    distribution.
 *
 *    Neither the name of Texas Instruments Incorporated nor the names of
 *    its contributors may be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *
 *  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 *  "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 *  LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 *  A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 *  OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 *  SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 *  LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 *  DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 *  THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 *  (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 *  OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
*/
#include "ADS1120.h"

static unsigned char RcvData[8];

SPI_HandleTypeDef* Adsspi;

static int OffsetCalibrateData = 0;
const float TheoreticalCodeValue = 8078378.1533;   // for 390 Ohm resistor

float ADS1120GainCorrection = 1.0;            // Gain correction factor; needs to be calculated once for any system
unsigned char ReadConversionData;
float PgaGainLevel;

void SPI_Error_Check(HAL_StatusTypeDef status)
{
	switch(status)
	{
		case HAL_TIMEOUT:
			printf("timeout\r\n");
		break;
		case HAL_BUSY:
			printf("busy\r\n");
		break;
		case HAL_ERROR:
			printf("error\r\n");
		break;
		default:
			printf("OK\r\n");
	}
}

/*************************************************************************************************************************************************
*  ADS1120_Reset
**************************************************************************************************************************************************/
void ADS1120_Reset (void)
{
    unsigned char cmd = ADS1120_RESET_CMD;
	HAL_StatusTypeDef sta;
	if((sta = HAL_SPI_TransmitReceive(Adsspi,&cmd,RcvData,1,0x1000))!=HAL_OK)
	{
		printf("Reset transmit error:");
		SPI_Error_Check(sta);
	}
}

/*************************************************************************************************************************************************
*  ADS1120_Start
**************************************************************************************************************************************************/
void ADS1120_Start (void)
{
    unsigned char cmd = ADS1120_START_CMD;
	HAL_StatusTypeDef sta;
	if((sta=HAL_SPI_TransmitReceive(Adsspi,&cmd,RcvData,1,0x1000))!=HAL_OK)
	{
		printf("Start transmit error:");
		SPI_Error_Check(sta);
	}
}

/*************************************************************************************************************************************************
*  ADS1120_Powerdown
**************************************************************************************************************************************************/
void ADS1120_Powerdown (void)
{
    unsigned char cmd = ADS1120_POWERDOWN_CMD;
	HAL_SPI_TransmitReceive(Adsspi,&cmd,RcvData,1,0x1000);
}

/*************************************************************************************************************************************************
*  ADS1120_Write_Regs
**************************************************************************************************************************************************/
void ADS1120_Write_Regs (unsigned char *writeValues, unsigned char startReg, unsigned char length)
{
    unsigned char outData[5];
    unsigned char i;
	HAL_StatusTypeDef sta;
    outData[0] = ADS1120_WRITE_CMD(startReg, length);

    for (i=0; i<length; i++)
    {
        outData[i+1] = writeValues[i];
    }

	if((sta=HAL_SPI_TransmitReceive(Adsspi,outData,RcvData,length+1,0x1000))!=HAL_OK)// Add 1 to length for command byte
	{
		printf("Transmit error:");
		SPI_Error_Check(sta);
	}
		
}

/*************************************************************************************************************************************************
*  ADS1120_Read_Regs
**************************************************************************************************************************************************/
void ADS1120_Read_Regs (unsigned char *readValues, unsigned char startReg, unsigned char length)
{
    unsigned char outData[5] = {0x55, 0x55, 0x55, 0x55, 0x55};
	HAL_StatusTypeDef sta;

    outData[0] = ADS1120_READ_CMD(startReg,length);

	if((sta=HAL_SPI_TransmitReceive(Adsspi,outData,readValues,length+1,0x1000))!=HAL_OK)
	{
		printf("Receive error:");
		SPI_Error_Check(sta);
	}
}

///*************************************************************************************************************************************************
//*  ADS1120_Send_Read_Data_Command
//**************************************************************************************************************************************************/
//void ADS1120_Send_Read_Data_Command (void)
//{
//    unsigned char cmd = ADS1120_RDATA_CMD;
//    USCIA1_SPI_Write (&cmd, RcvData, 1);
//}


/*************************************************************************************************************************************************
*  ADS1120_Get_Conversion_Data
**************************************************************************************************************************************************/
void ADS1120_Get_Conversion_Data (unsigned char *conversionData)
{
    unsigned char outData[2] = {0xff, 0xff};
	HAL_StatusTypeDef sta;

	if((sta=HAL_SPI_TransmitReceive(Adsspi,outData,conversionData,2,0x1000))!=HAL_OK)// 2 Bytes of Conversion Data
	{
		printf("Get Conversion Data error:");
		SPI_Error_Check(sta);
	}
}

/*************************************************************************************************************************************************
*  Setup_ADS1120
**************************************************************************************************************************************************/
/*!
* @brief Performs the setup of the ADS1120.
*
* This function will configure the ADS1120.
*
* @param[in]     inputMux        Mux settings for the AIN pins (ADS1120_MUX_AIN0_AIN1, ADS1120_MUX_AIN0_AIN2, ADS1120_MUX_AIN0_AIN3,
*                                                               ADS1120_MUX_AIN1_AIN2, ADS1120_MUX_AIN1_AIN3, ADS1120_MUX_AIN2_AIN3,
*                                                               ADS1120_MUX_AIN1_AIN0, ADS1120_MUX_AIN3_AIN2, ADS1120_MUX_AIN0_AVSS,
*                                                               ADS1120_MUX_AIN1_AVSS, ADS1120_MUX_AIN2_AVSS, ADS1120_MUX_AIN3_AVSS,
*                                                               ADS1120_MUX_REFP-REFN, ADS1120_MUX_AVDD-AVSS, ADS1120_MUX_SHORTED)
* @param[in]     opMode          ADS1120 operating mode (	, ADS1120_OP_MODE_DUTY, ADS1120_OP_MODE_TURBO)
* @param[in]     conversionMode  Identifies single shot or continuous conversions (ADS1120_CONVERSION_SINGLE_SHOT or ADS1120_CONVERSION_CONTINUOUS)
* @param[in]     dataRate        ADS1120 Data Rate (ADS1120_DATA_RATE_20SPS, ADS1120_DATA_RATE_45SPS, ADS1120_DATA_RATE_90SPS,
*                                                   ADS1120_DATA_RATE_175SPS, ADS1120_DATA_RATE_330SPS, ADS1120_DATA_RATE_600SPS, or
*                                                   ADS1120_DATA_RATE_1000SPS)
* @param[in]     gainLevel       ADS1120 Gain Level (ADS1120_GAIN_1, ADS1120_GAIN_2, ADS1120_GAIN_4, ADS1120_GAIN_8, ADS1120_GAIN_16,
*                                                    ADS1120_GAIN_32, ADS1120_GAIN_64, or ADS1120_GAIN_128)
* @param[in]     pgaBypass       Bypass PGA for gain settings - Note the PGA will be used whenever the gain setting is ADS1120_GAIN_8 or greater,
*                                no matter what this value is set to. (ADS1120_PGA_BYPASS or ADS1120_USE_PGA)
* @param[in]     routeIDAC1      The output pin used by IDAC1 (ADS1120_IDAC1_DISABLED, ADS1120_IDAC1_AIN0, ADS1120_IDAC1_AIN1, ADS1120_IDAC1_AIN2,
*                                                              ADS1120_IDAC1_AIN3, ADS1120_IDAC1_REFP, or ADS1120_IDAC1_REFN
* @param[in]     routeIDAC2      The output pin used by IDAC2 (ADS1120_IDAC2_DISABLED, ADS1120_IDAC2_AIN0, ADS1120_IDAC2_AIN1, ADS1120_IDAC2_AIN2,
*                                                              ADS1120_IDAC2_AIN3, ADS1120_IDAC2_REFP, or ADS1120_IDAC2_REFN
* @param[in]     idacCurrent     The IDAC Biasing Current (ADS1120_IDAC_CURRENT_OFF, ADS1120_IDAC_CURRENT_10_UA, ADS1120_IDAC_CURRENT_50_UA,
*                                                          ADS1120_IDAC_CURRENT_100_UA, ADS1120_IDAC_CURRENT_250_UA, ADS1120_IDAC_CURRENT_500_UA,
*                                                          ADS1120_IDAC_CURRENT_1000_UA, ADS1120_IDAC_CURRENT_1500_UA
*
* @return  None
*
* @sa Setup_ADS1120_CS()
**************************************************************************************************************************************************/
void Setup_ADS1120 (SPI_HandleTypeDef* hspi,unsigned char inputMux, unsigned char opMode,
                    unsigned char conversionMode, unsigned char dataRate, unsigned char gainLevel, unsigned char pgaBypass,
                    unsigned char routeIDAC1, unsigned char routeIDAC2, unsigned char idacCurrent)

{

    unsigned char config[4];
	Adsspi = hspi;
    // Reset the ADS1120
    ADS1120_Reset();
	
    config[0] = inputMux + gainLevel + pgaBypass;
    config[1] = dataRate + opMode + conversionMode + ADS1120_TEMP_SENSOR_OFF + ADS1120_BURN_OUT_CURRENT_OFF;
    config[2] = ADS1120_FIR_50_60 + ADS1120_VREF_EXT_REF0_PINS + ADS1120_LOW_SIDE_POWER_OPEN + idacCurrent;
    config[3] = routeIDAC1 + routeIDAC2 + ADS1120_DRDY_ON_DRDY_ONLY;
	HAL_Delay(1);
	switch (gainLevel)
    {
        case ADS1120_GAIN_1:
            PgaGainLevel = 1.0;
            break;
        case ADS1120_GAIN_2:
            PgaGainLevel = 2.0;
            break;
        case ADS1120_GAIN_4:
            PgaGainLevel = 4.0;
            break;
        case ADS1120_GAIN_8:
            PgaGainLevel = 8.0;
            break;
        case ADS1120_GAIN_16:
            PgaGainLevel = 16.0;
            break;
        case ADS1120_GAIN_32:
            PgaGainLevel = 32.0;
            break;
        case ADS1120_GAIN_64:
            PgaGainLevel = 64.0;
            break;
        case ADS1120_GAIN_128:
            PgaGainLevel = 128.0;
            break;

    }
    ADS1120_Write_Regs (config, ADS1120_CONFIG_0_REG, 4);
}

/*************************************************************************************************************************************************
*  ADS1120_Offset_Calibrate_Data
**************************************************************************************************************************************************/
#define CalibrateCount 8

void ADS1120_Offset_Calibrate_Data (void)
{
    unsigned char index = 0;
    signed int temp;
    unsigned char tempData[2];

    // Determine the ADS1120 Calibration offset - Short the AIN0 and AIN1 together and measure the results 8 times.
    // The average result will be subtracted from all future measurements.
    Setup_ADS1120 (Adsspi,ADS1120_MUX_SHORTED, ADS1120_OP_MODE_NORMAL,
                   ADS1120_CONVERSION_SINGLE_SHOT, ADS1120_DATA_RATE_20SPS, ADS1120_GAIN_16, ADS1120_USE_PGA,
                   ADS1120_IDAC1_AIN2, ADS1120_IDAC2_AIN3, ADS1120_IDAC_CURRENT_250_UA);

    // Delay 500ms to allow for IDAC and Input Filter settling
    HAL_Delay(500);

    ReadConversionData = 0;

    for(index=CalibrateCount;index>0;index--)
    {
        ADS1120_Start ();             // Kick off conversion
        while (!ReadConversionData);  // Wait for Data Ready interrupt
        ReadConversionData = 0;
        ADS1120_Get_Conversion_Data ((unsigned char *)tempData);   // Get the raw data
        temp = ((int)tempData[0] << 8) + tempData[1];

        OffsetCalibrateData += temp;
    }
    OffsetCalibrateData = (OffsetCalibrateData >> 3);  // OffsetCalibrateData / 8
//    OffsetCalibrateData += 18;
}

/*************************************************************************************************************************************************
*  ADS1120_Offset_Calibrate_Data
**************************************************************************************************************************************************/
void ADS1120_Gain_Calibrate_Data (void)
{
    unsigned char index = 0;
    unsigned char tempData[2];
    int i32ConversionData;
    float temp = 0;
    float temp1 = 0;

    // Determine the ADS1120 Calibration offset - Short the AIN0 and AIN1 together and measure the results 8 times.
    // The average result will be subtracted from all future measurements.
    Setup_ADS1120 (Adsspi,ADS1120_MUX_AIN1_AIN0, ADS1120_OP_MODE_NORMAL,
                   ADS1120_CONVERSION_SINGLE_SHOT, ADS1120_DATA_RATE_20SPS, ADS1120_GAIN_16, ADS1120_USE_PGA,
                   ADS1120_IDAC1_AIN2, ADS1120_IDAC2_AIN3, ADS1120_IDAC_CURRENT_250_UA);

    // Delay 500ms to allow for IDAC and Input Filter settling
    HAL_Delay(500);

    ReadConversionData = 0;

    for(index=CalibrateCount;index>0;index--)
    {
        ADS1120_Start ();             // Kick off conversion
        while (!ReadConversionData);  // Wait for Data Ready interrupt
        ReadConversionData = 0;
        ADS1120_Get_Conversion_Data ((unsigned char *)tempData);   // Get the raw data
        i32ConversionData = ((int)tempData[0] << 8) + tempData[1];

        i32ConversionData -= OffsetCalibrateData;
        temp = TheoreticalCodeValue / i32ConversionData;
        temp1 += temp;
    }
    //SYSCFG0 = FRWPPW | DFWP;            // Program FRAM write enable
    ADS1120GainCorrection = temp1 / 8;
    //SYSCFG0 = FRWPPW | PFWP | DFWP;     // Program FRAM write protected (not writable)
}

/*************************************************************************************************************************************************
*  ADS1120_Get_Conversion_Data_Calibrated
**************************************************************************************************************************************************/
int ADS1120_Get_Conversion_Data_Calibrated (void)
{
    unsigned char tempData[2];
    int i32ConversionData;

    ADS1120_Get_Conversion_Data ((unsigned char *)tempData);

    i32ConversionData = ((int)tempData[0] << 8) + tempData[1];

    i32ConversionData = i32ConversionData - OffsetCalibrateData;

    if(i32ConversionData < 0) i32ConversionData = 0x7FFF;

    // Make sure that there is a value in place for the Gain
    if (*(unsigned long *)&ADS1120GainCorrection != 0xffffffff)
        i32ConversionData = (long) ((float)i32ConversionData * ADS1120GainCorrection);

    return i32ConversionData;
}
