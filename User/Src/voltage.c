#include "voltage.h"

/**
 * @brief  读取指定 ADC 通道的电压值（单位：伏特）
 * @param  hadc: ADC 句柄指针（如 &hadc1）
 * @retval 电压值（float，单位 V），若出错返回 -1.0f
 */
float ADC_ReadVoltage(ADC_HandleTypeDef* hadc) {
	if (HAL_ADC_Start(hadc) != HAL_OK)
    return -1.0f;
	if (HAL_ADC_PollForConversion(hadc, 1000) != HAL_OK) 
		return -1.0f;
	uint32_t raw = HAL_ADC_GetValue(hadc);
	HAL_ADC_Stop(hadc);
	return ((raw * 3.3 / 4095.0f) * 2 - 3) / 1 * 100;
}

/**
 * @brief 校准
 */
void ADC_Calibration(void) {
	HAL_ADCEx_Calibration_Start(&hadc1, ADC_CALIB_OFFSET, ADC_SINGLE_ENDED);
	// HAL_ADCEx_Calibration_Start(&hadc2, ADC_SINGLE_ENDED);
}