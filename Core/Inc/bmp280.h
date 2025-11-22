/*
 * bmp280.h
 *
 *  Created on: Nov 17, 2025
 *      Author: admin
 */

#ifndef BMP280_H_
#define BMP280_H_


#include "stm32l4xx_hal.h"
#include <stdint.h>

/* Dirección I2C típica (7-bit). Ajustá si tu módulo usa 0x77 */
#define BMP280_I2C_ADDR        (0x77 << 1)

/* Registros */
#define BMP280_REG_CHIPID      0xD0
#define BMP280_REG_RESET       0xE0
#define BMP280_REG_STATUS      0xF3
#define BMP280_REG_CTRL_MEAS   0xF4
#define BMP280_REG_CONFIG      0xF5
#define BMP280_REG_PRESS_MSB   0xF7  // 0xF7..0xF9 pressure, 0xFA..0xFC temp
#define BMP280_REG_TEMP_MSB    0xFA

/* Calibration data block */
typedef struct {
    uint16_t dig_T1;
    int16_t  dig_T2;
    int16_t  dig_T3;

    uint16_t dig_P1;
    int16_t  dig_P2;
    int16_t  dig_P3;
    int16_t  dig_P4;
    int16_t  dig_P5;
    int16_t  dig_P6;
    int16_t  dig_P7;
    int16_t  dig_P8;
    int16_t  dig_P9;
} BMP280_Calib_t;

/* API */
HAL_StatusTypeDef BMP280_ReadID(uint8_t *id);
HAL_StatusTypeDef BMP280_Init(void);
HAL_StatusTypeDef BMP280_ReadCalibration(void);

HAL_StatusTypeDef BMP280_ReadRawTemperature(int32_t *raw_temp);
HAL_StatusTypeDef BMP280_ReadRawPressure(int32_t *raw_press);

/* Compensation functions; return float physical units */
float BMP280_CompensateTemperature(int32_t raw_temp); // °C
float BMP280_CompensatePressure(int32_t raw_press);    // hPa




#endif /* BMP280_H_ */
