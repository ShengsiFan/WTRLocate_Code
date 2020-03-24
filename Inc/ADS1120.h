/*
 *    ADS1120.h
 *
 *    Header file for ADS1120.c library software
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

#ifndef ADS1120_H_
#define ADS1120_H_
#include "main.h"

extern unsigned char ReadConversionData;
extern float PgaGainLevel;
extern float ADS1220GainCorrection;


#define ADS1120_RESET_CMD          0x06
#define ADS1120_WREG_CMD           0x40
#define ADS1120_RREG_CMD           0x20
#define ADS1120_RDATA_CMD          0x10
#define ADS1120_START_CMD          0x08
#define ADS1120_POWERDOWN_CMD      0x02


#define ADS1120_WRITE_CMD(reg,len)     (ADS1120_WREG_CMD + (reg << 2) + (len-1))
#define ADS1120_READ_CMD(reg,len)      (ADS1120_RREG_CMD + (reg << 2) + (len-1))


#define ADS1120_CONFIG_0_REG      0
#define ADS1120_CONFIG_1_REG      1
#define ADS1120_CONFIG_2_REG      2
#define ADS1120_CONFIG_3_REG      3


// Configuration Register 0 Settings  00h
#define ADS1120_MUX_MASK          0xf0
#define ADS1120_MUX_SHIFT         4

#define ADS1120_MUX_AIN0_AIN1     (0x0 << ADS1120_MUX_SHIFT)
#define ADS1120_MUX_AIN0_AIN2     (0x1 << ADS1120_MUX_SHIFT)
#define ADS1120_MUX_AIN0_AIN3     (0x2 << ADS1120_MUX_SHIFT)
#define ADS1120_MUX_AIN1_AIN2     (0x3 << ADS1120_MUX_SHIFT)
#define ADS1120_MUX_AIN1_AIN3     (0x4 << ADS1120_MUX_SHIFT)
#define ADS1120_MUX_AIN2_AIN3     (0x5 << ADS1120_MUX_SHIFT)
#define ADS1120_MUX_AIN1_AIN0     (0x6 << ADS1120_MUX_SHIFT)
#define ADS1120_MUX_AIN3_AIN2     (0x7 << ADS1120_MUX_SHIFT)
#define ADS1120_MUX_AIN0_AVSS     (0x8 << ADS1120_MUX_SHIFT)
#define ADS1120_MUX_AIN1_AVSS     (0x9 << ADS1120_MUX_SHIFT)
#define ADS1120_MUX_AIN2_AVSS     (0xa << ADS1120_MUX_SHIFT)
#define ADS1120_MUX_AIN3_AVSS     (0xb << ADS1120_MUX_SHIFT)
#define ADS1120_MUX_REFPmREFN     (0xc << ADS1120_MUX_SHIFT)
#define ADS1120_MUX_AVDDmAVSS     (0xd << ADS1120_MUX_SHIFT)
#define ADS1120_MUX_SHORTED       (0xe << ADS1120_MUX_SHIFT)


#define ADS1120_GAIN_MASK         0xe
#define ADS1120_GAIN_SHIFT        1

#define ADS1120_GAIN_1            (0x0 << ADS1120_GAIN_SHIFT)
#define ADS1120_GAIN_2            (0x1 << ADS1120_GAIN_SHIFT)
#define ADS1120_GAIN_4            (0x2 << ADS1120_GAIN_SHIFT)
#define ADS1120_GAIN_8            (0x3 << ADS1120_GAIN_SHIFT)
#define ADS1120_GAIN_16           (0x4 << ADS1120_GAIN_SHIFT)
#define ADS1120_GAIN_32           (0x5 << ADS1120_GAIN_SHIFT)
#define ADS1120_GAIN_64           (0x6 << ADS1120_GAIN_SHIFT)
#define ADS1120_GAIN_128          (0x7 << ADS1120_GAIN_SHIFT)

#define ADS1120_PGA_BYPASS        1
#define ADS1120_USE_PGA           0


// Configuration Register 1 Settings   01h

#define ADS1120_DATA_RATE_MASK    0xe0
#define ADS1120_DATA_RATE_SHIFT   5

#define ADS1120_DATA_RATE_20SPS   (0 << ADS1120_DATA_RATE_SHIFT)
#define ADS1120_DATA_RATE_45SPS   (1 << ADS1120_DATA_RATE_SHIFT)
#define ADS1120_DATA_RATE_90SPS   (2 << ADS1120_DATA_RATE_SHIFT)
#define ADS1120_DATA_RATE_175SPS  (3 << ADS1120_DATA_RATE_SHIFT)
#define ADS1120_DATA_RATE_330SPS  (4 << ADS1120_DATA_RATE_SHIFT)
#define ADS1120_DATA_RATE_600SPS  (5 << ADS1120_DATA_RATE_SHIFT)
#define ADS1120_DATA_RATE_1000SPS (6 << ADS1120_DATA_RATE_SHIFT)

#define ADS1120_OP_MODE_MASK      0x18
#define ADS1120_OP_MODE_SHIFT     3

#define ADS1120_OP_MODE_NORMAL    (0 << ADS1120_OP_MODE_SHIFT)
#define ADS1120_OP_MODE_DUTY      (1 << ADS1120_OP_MODE_SHIFT)
#define ADS1120_OP_MODE_TURBO     (2 << ADS1120_OP_MODE_SHIFT)

#define ADS1120_CONVERSION_MODE_MASK     0x4
#define ADS1120_CONVERSION_SINGLE_SHOT   0x0
#define ADS1120_CONVERSION_CONTINUOUS    0x4

#define ADS1120_TEMP_SENSOR_MASK         0x2
#define ADS1120_TEMP_SENSOR_ON           0x2
#define ADS1120_TEMP_SENSOR_OFF          0x0

#define ADS1120_BURN_OUT_MASK            0x1
#define ADS1120_BURN_OUT_CURRENT_ON      0x1
#define ADS1120_BURN_OUT_CURRENT_OFF     0x0

// Configuration Register 2 Settings   02h

#define ADS1120_VREF_MASK                0xc0
#define ADS1120_VREF_SHIFT               6

#define ADS1120_VREF_INTERNAL            (0 << ADS1120_VREF_SHIFT)
#define ADS1120_VREF_EXT_REF0_PINS       (1 << ADS1120_VREF_SHIFT)
#define ADS1120_VREF_EXT_REF1_PINS       (2 << ADS1120_VREF_SHIFT)
#define ADS1120_VREF_AVDD                (3 << ADS1120_VREF_SHIFT)

#define ADS1120_FIR_FILTER_MASK          0x30
#define ADS1120_FIR_FILTER_SHIFT         4

#define ADS1120_FIR_NONE                 (0 << ADS1120_FIR_FILTER_SHIFT)
#define ADS1120_FIR_50_60                (1 << ADS1120_FIR_FILTER_SHIFT)
#define ADS1120_FIR_50                   (2 << ADS1120_FIR_FILTER_SHIFT)
#define ADS1120_FIR_60                   (3 << ADS1120_FIR_FILTER_SHIFT)

#define ADS1120_LOW_SIDE_POWER_MASK      0x04
#define ADS1120_LOW_SIDE_POWER_OPEN      0x00
#define ADS1120_LOW_SIDE_POWER_CLOSES    0x04

#define ADS1120_IDAC_CURRENT_MASK        0x07
#define ADS1120_IDAC_CURRENT_SHIFT       0

#define ADS1120_IDAC_CURRENT_OFF         (0 << ADS1120_IDAC_CURRENT_SHIFT)
#define ADS1120_IDAC_CURRENT_10_UA       (1 << ADS1120_IDAC_CURRENT_SHIFT)
#define ADS1120_IDAC_CURRENT_50_UA       (2 << ADS1120_IDAC_CURRENT_SHIFT)
#define ADS1120_IDAC_CURRENT_100_UA      (3 << ADS1120_IDAC_CURRENT_SHIFT)
#define ADS1120_IDAC_CURRENT_250_UA      (4 << ADS1120_IDAC_CURRENT_SHIFT)
#define ADS1120_IDAC_CURRENT_500_UA      (5 << ADS1120_IDAC_CURRENT_SHIFT)
#define ADS1120_IDAC_CURRENT_1000_UA     (6 << ADS1120_IDAC_CURRENT_SHIFT)
#define ADS1120_IDAC_CURRENT_1500_UA     (7 << ADS1120_IDAC_CURRENT_SHIFT)

// Configuration Register 3 Settings   03h

#define ADS1120_IDAC1_MUX_MASK           0xe0
#define ADS1120_IDAC1_MUX_SHIFT          5

#define ADS1120_IDAC1_DISABLED           (0 << ADS1120_IDAC1_MUX_SHIFT)
#define ADS1120_IDAC1_AIN0               (1 << ADS1120_IDAC1_MUX_SHIFT)
#define ADS1120_IDAC1_AIN1               (2 << ADS1120_IDAC1_MUX_SHIFT)
#define ADS1120_IDAC1_AIN2               (3 << ADS1120_IDAC1_MUX_SHIFT)
#define ADS1120_IDAC1_AIN3               (4 << ADS1120_IDAC1_MUX_SHIFT)
#define ADS1120_IDAC1_REFP0              (5 << ADS1120_IDAC1_MUX_SHIFT)
#define ADS1120_IDAC1_REFN0              (6 << ADS1120_IDAC1_MUX_SHIFT)

#define ADS1120_IDAC2_MUX_MASK           0x1c
#define ADS1120_IDAC2_MUX_SHIFT          2

#define ADS1120_IDAC2_DISABLED           (0 << ADS1120_IDAC2_MUX_SHIFT)
#define ADS1120_IDAC2_AIN0               (1 << ADS1120_IDAC2_MUX_SHIFT)
#define ADS1120_IDAC2_AIN1               (2 << ADS1120_IDAC2_MUX_SHIFT)
#define ADS1120_IDAC2_AIN2               (3 << ADS1120_IDAC2_MUX_SHIFT)
#define ADS1120_IDAC2_AIN3               (4 << ADS1120_IDAC2_MUX_SHIFT)
#define ADS1120_IDAC2_REFP0              (5 << ADS1120_IDAC2_MUX_SHIFT)
#define ADS1120_IDAC2_REFN0              (6 << ADS1120_IDAC2_MUX_SHIFT)

#define ADS1120_DOUT_DRDY_MASK           0x2
#define ADS1120_DRDY_ON_DOUT_DRDY        0x2
#define ADS1120_DRDY_ON_DRDY_ONLY        0x0

void Setup_ADS1120 (SPI_HandleTypeDef* hspi, unsigned char inputMux, unsigned char opMode,
                           unsigned char conversionMode, unsigned char dataRate, unsigned char gainLevel, unsigned char pgaBypass,
                           unsigned char routeIDAC1, unsigned char routeIDAC2, unsigned char idacCurrent);
void ADS1120_Start (void);
void ADS1120_Reset (void);
void ADS1120_Powerdown (void);
void ADS1120_Write_Regs (unsigned char *writeValues, unsigned char startReg, unsigned char length);
void ADS1120_Read_Regs (unsigned char *readValues, unsigned char startReg, unsigned char length);
void ADS1120_Get_Conversion_Data (unsigned char *conversionData);
void ADS1120_Offset_Calibrate_Data (void);
void ADS1120_Gain_Calibrate_Data (void);
int ADS1120_Get_Conversion_Data_Calibrated (void);


#endif /* ADS1120_H_ */
