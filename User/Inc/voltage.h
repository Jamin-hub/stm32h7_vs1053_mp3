#ifndef __VOLTAGE_H
#define __VOLTAGE_H

#include "adc.h"

float ADC_ReadVoltage(ADC_HandleTypeDef* hadc);
void ADC_Calibration(void);

#endif
