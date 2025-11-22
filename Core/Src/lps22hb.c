/*
 * lps22hb.c
 *
 *  Created on: Nov 22, 2025
 *      Author: admin
 */

#include "lps22hb.h"
#include "i2c_driver.h"
#include <string.h>

/* Optional: change the address if your board uses 0x5D */
#ifndef LPS22HB_I2C_ADDR
#define LPS22HB_I2C_ADDR (0x5C << 1)
#endif

HAL_StatusTypeDef LPS22HB_ReadID(uint8_t *id)
{
    return I2C_ReadReg(LPS22HB_I2C_ADDR, LPS22HB_WHO_AM_I, id);
}

HAL_StatusTypeDef LPS22HB_Init(void)
{
    HAL_StatusTypeDef st;

    /* CTRL_REG1: set ODR and BDU = 1 (block data update)
       Example: ODR = 0b010 (10 Hz) -> (ODR << 4). BDU bit = bit 2 (value 1<<2) ?
       Implementation from datasheet: CTRL_REG1 (0x10):
         ODR[2:0] bits at 6:4, BDU at bit 2.
       We'll set ODR = 0b010 (10 Hz) and BDU = 1.
    */
    uint8_t ctrl1 = (0x02 << 4) | (1 << 2); // ODR=010 (10 Hz), BDU=1
    st = I2C_WriteReg(LPS22HB_I2C_ADDR, LPS22HB_CTRL_REG1, ctrl1);
    if (st != HAL_OK) return st;

    HAL_Delay(20); // give time to apply

    /* CTRL_REG2: soft reset / one-shot not needed here; leave default (0) */
    return HAL_OK;
}

/* Read 3 bytes PRESS_OUT_XL..H, build signed 24-bit */
HAL_StatusTypeDef LPS22HB_ReadRawPressure(int32_t *raw_press)
{
    uint8_t buf[3];
    HAL_StatusTypeDef st = I2C_ReadMulti(LPS22HB_I2C_ADDR, LPS22HB_PRESS_OUT_XL | 0x80, buf, 3);
    if (st != HAL_OK) return st;

    uint32_t tmp = ((uint32_t)buf[0]) | (((uint32_t)buf[1]) << 8) | (((uint32_t)buf[2]) << 16);
    /* convert 24-bit two's complement to 32-bit signed */
    if (tmp & 0x00800000) {
        tmp |= 0xFF000000;
    }
    *raw_press = (int32_t)tmp;
    return HAL_OK;
}

/* Read 2 bytes TEMP_OUT_L..H and build signed 16-bit */
HAL_StatusTypeDef LPS22HB_ReadRawTemperature(int16_t *raw_temp)
{
    uint8_t buf[2];
    HAL_StatusTypeDef st = I2C_ReadMulti(LPS22HB_I2C_ADDR, LPS22HB_TEMP_OUT_L | 0x80, buf, 2);
    if (st != HAL_OK) return st;

    int16_t t = (int16_t)((buf[1] << 8) | buf[0]);
    *raw_temp = t;
    return HAL_OK;
}

/* Conversions */
float LPS22HB_Pressure_hPa_from_raw(int32_t raw_press)
{
    /* According to datasheet: P(hPa) = raw / 4096 (LSB/hPa = 4096) */
    return ((float)raw_press) / 4096.0f;
}

float LPS22HB_Temp_degC_from_raw(int16_t raw_temp)
{
    /* According to datasheet: T(°C) = raw_temp / 100 */
    return ((float)raw_temp) / 100.0f;
}

