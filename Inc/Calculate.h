
#ifndef CALCULATE_H
#define CALCULATE_H

#include "ADS8328.h"
#include "ADS1120.h"
#include "Encoder.h"
#include "math.h"

void RunDevices(void);
void Calculate(void);

extern float Robot_X,Robot_Y,Robot_Angle;

#endif
