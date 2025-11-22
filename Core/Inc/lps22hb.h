/*
 * lps22hb.h
 *
 *  Created on: Nov 22, 2025
 *      Author: admin
 */

#ifndef INC_LPS22HB_H_
#define INC_LPS22HB_H_


#include "stm32l4xx_hal.h"
#include <stdint.h>

/* I2C address: try 0x5C or 0x5D (7-bit). We'll define default as 0x5C */
#define LPS22HB_I2C_ADDR      (0x5D << 1)

/* Registers */
#define LPS22HB_WHO_AM_I      0x0F
#define LPS22HB_CTRL_REG1     0x10
#define LPS22HB_CTRL_REG2     0x11
#define LPS22HB_STATUS        0x27
#define LPS22HB_PRESS_OUT_XL  0x28
#define LPS22HB_PRESS_OUT_L   0x29
#define LPS22HB_PRESS_OUT_H   0x2A
#define LPS22HB_TEMP_OUT_L    0x2B
#define LPS22HB_TEMP_OUT_H    0x2C

/* API */
HAL_StatusTypeDef LPS22HB_ReadID(uint8_t *id);
HAL_StatusTypeDef LPS22HB_Init(void);
HAL_StatusTypeDef LPS22HB_ReadRawPressure(int32_t *raw_press);
HAL_StatusTypeDef LPS22HB_ReadRawTemperature(int16_t *raw_temp);

/* Converters */
float LPS22HB_Pressure_hPa_from_raw(int32_t raw_press);
float LPS22HB_Temp_degC_from_raw(int16_t raw_temp);



#endif /* INC_LPS22HB_H_ */
