/*
 * hts221.h
 *
 *  Created on: Nov 17, 2025
 *      Author: admin
 */

#ifndef HTS221_H_
#define HTS221_H_

#include <stdint.h>
#include "stm32l4xx_hal.h"

#define HTS221_I2C_ADDR      (0x5F << 1)
#define HTS221_WHO_AM_I_REG  0x0F
#define HTS221_CTRL_REG1      0x20
#define HTS221_CTRL_REG2      0x21
#define HTS221_AV_CONF        0x10

#define HTS221_H_OUT_L   0x28
#define HTS221_T_OUT_L   0x2A
#define HTS221_AUTO_INCREMENT   0x80


// Estructura de calibración
typedef struct {
    float H0_rH;
    float H1_rH;
    int16_t H0_T0_OUT;
    int16_t H1_T0_OUT;

    float T0_degC;
    float T1_degC;
    int16_t T0_OUT;
    int16_t T1_OUT;
} HTS221_Calib_t;

// PROTOTIPOS
HAL_StatusTypeDef HTS221_ReadWhoAmI(uint8_t *id);
HAL_StatusTypeDef HTS221_Init(void);
HAL_StatusTypeDef HTS221_ReadCalibration(void);

HAL_StatusTypeDef HTS221_ReadRawHumidity(int16_t *raw);
HAL_StatusTypeDef HTS221_ReadRawTemperature(int16_t *raw);

float HTS221_ComputeHumidity(int16_t raw);
float HTS221_ComputeTemperature(int16_t raw);

#endif /* HTS221_H_ */
