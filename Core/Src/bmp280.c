/*
 * bmp280.c
 *
 *  Created on: Nov 17, 2025
 *      Author: admin
 */


#include "bmp280.h"
#include "i2c_driver.h"  // asumimos tu API: I2C_ReadReg/I2C_ReadMulti/I2C_WriteReg
#include <string.h>

static BMP280_Calib_t bmp_calib;
static int32_t bmp_t_fine = 0;

/* Leer ID */
HAL_StatusTypeDef BMP280_ReadID(uint8_t *id)
{
    return I2C_ReadReg(BMP280_I2C_ADDR, BMP280_REG_CHIPID, id);
}

/* Inicialización básica: soft reset + set oversampling & mode */
HAL_StatusTypeDef BMP280_Init(void)
{
    HAL_StatusTypeDef st;
    uint8_t reset = 0xB6;
    st = I2C_WriteReg(BMP280_I2C_ADDR, BMP280_REG_RESET, reset);
    if (st != HAL_OK) return st;

    HAL_Delay(100);

    /* config: t_sb=0 (0.5ms), filter=4 (x16) -> example */
    uint8_t config = (0 << 5) | (0x04 << 2); // t_sb=0, filter=4
    st = I2C_WriteReg(BMP280_I2C_ADDR, BMP280_REG_CONFIG, config);
    if (st != HAL_OK) return st;

    /* ctrl_meas: osrs_t=1 (x1), osrs_p=1 (x1), mode=normal(11) */
    uint8_t ctrl_meas = (1 << 5) | (1 << 2) | (3); // T oversampling 1, P oversampling 1, normal mode
    st = I2C_WriteReg(BMP280_I2C_ADDR, BMP280_REG_CTRL_MEAS, ctrl_meas);
    if (st != HAL_OK) return st;

    HAL_Delay(100);

    return BMP280_ReadCalibration();
}

/* Leer bloque de calibración */
HAL_StatusTypeDef BMP280_ReadCalibration(void)
{
    uint8_t buf[24];
    HAL_StatusTypeDef st;

    /* Calibration registers from 0x88 to 0xA1 (24 bytes for temp & press calib) */
    st = I2C_ReadMulti(BMP280_I2C_ADDR, 0x88, buf, 24);
    if (st != HAL_OK) return st;

    bmp_calib.dig_T1 = (uint16_t)(buf[1] << 8 | buf[0]);
    bmp_calib.dig_T2 = (int16_t)(buf[3] << 8 | buf[2]);
    bmp_calib.dig_T3 = (int16_t)(buf[5] << 8 | buf[4]);

    bmp_calib.dig_P1 = (uint16_t)(buf[7] << 8 | buf[6]);
    bmp_calib.dig_P2 = (int16_t)(buf[9] << 8 | buf[8]);
    bmp_calib.dig_P3 = (int16_t)(buf[11] << 8 | buf[10]);
    bmp_calib.dig_P4 = (int16_t)(buf[13] << 8 | buf[12]);
    bmp_calib.dig_P5 = (int16_t)(buf[15] << 8 | buf[14]);
    bmp_calib.dig_P6 = (int16_t)(buf[17] << 8 | buf[16]);
    bmp_calib.dig_P7 = (int16_t)(buf[19] << 8 | buf[18]);
    bmp_calib.dig_P8 = (int16_t)(buf[21] << 8 | buf[20]);
    bmp_calib.dig_P9 = (int16_t)(buf[23] << 8 | buf[22]);

    return HAL_OK;
}

/* Leer 20-bit raw temperature from 0xFA..0xFC */
HAL_StatusTypeDef BMP280_ReadRawTemperature(int32_t *raw_temp)
{
    uint8_t buf[3];
    HAL_StatusTypeDef st = I2C_ReadMulti(BMP280_I2C_ADDR, BMP280_REG_TEMP_MSB, buf, 3);
    if (st != HAL_OK) return st;

    *raw_temp = (int32_t)((((int32_t)buf[0]) << 12) | (((int32_t)buf[1]) << 4) | ((int32_t)(buf[2] >> 4)));
    return HAL_OK;
}

/* Leer 20-bit raw pressure from 0xF7..0xF9 */
HAL_StatusTypeDef BMP280_ReadRawPressure(int32_t *raw_press)
{
    uint8_t buf[3];
    HAL_StatusTypeDef st = I2C_ReadMulti(BMP280_I2C_ADDR, BMP280_REG_PRESS_MSB, buf, 3);
    if (st != HAL_OK) return st;

    *raw_press = (int32_t)((((int32_t)buf[0]) << 12) | (((int32_t)buf[1]) << 4) | ((int32_t)(buf[2] >> 4)));
    return HAL_OK;
}

/* Compensation per datasheet (integer math). Fill bmp_t_fine for pressure use. Returns °C */
float BMP280_CompensateTemperature(int32_t raw_temp)
{
    /* Following datasheet formula */
    int32_t var1, var2;
    var1 = ((((raw_temp >> 3) - ((int32_t)bmp_calib.dig_T1 << 1))) * ((int32_t)bmp_calib.dig_T2)) >> 11;
    var2 = (((((raw_temp >> 4) - ((int32_t)bmp_calib.dig_T1)) * ((raw_temp >> 4) - ((int32_t)bmp_calib.dig_T1))) >> 12) * ((int32_t)bmp_calib.dig_T3)) >> 14;
    bmp_t_fine = var1 + var2;
    float T = (bmp_t_fine * 5 + 128) >> 8;
    return T / 100.0f; // °C
}

/* Compensation for pressure; returns hPa */
float BMP280_CompensatePressure(int32_t raw_press)
{
    int64_t var1, var2, p;
    var1 = ((int64_t)bmp_t_fine) - 128000;
    var2 = var1 * var1 * (int64_t)bmp_calib.dig_P6;
    var2 = var2 + ((var1 * (int64_t)bmp_calib.dig_P5) << 17);
    var2 = var2 + (((int64_t)bmp_calib.dig_P4) << 35);
    var1 = ((var1 * var1 * (int64_t)bmp_calib.dig_P3) >> 8) + ((var1 * (int64_t)bmp_calib.dig_P2) << 12);
    var1 = (((((int64_t)1) << 47) + var1)) * ((int64_t)bmp_calib.dig_P1) >> 33;

    if (var1 == 0) {
        return 0; // avoid division by zero
    }
    p = 1048576 - raw_press;
    p = (((p << 31) - var2) * 3125) / var1;
    var1 = (((int64_t)bmp_calib.dig_P9) * (p >> 13) * (p >> 13)) >> 25;
    var2 = (((int64_t)bmp_calib.dig_P8) * p) >> 19;

    p = ((p + var1 + var2) >> 8) + (((int64_t)bmp_calib.dig_P7) << 4);

    /* p is in Q24.8 (Pa) -> convert to hPa */
    float pres_hpa = (float)p / 25600.0f; // because p/256 gives Pa with extra factor
    return pres_hpa;
}
