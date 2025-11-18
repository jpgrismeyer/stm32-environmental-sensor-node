/*
 * hts221.c
 *
 *  Created on: Nov 17, 2025
 *      Author: admin
 */


#include "hts221.h"
#include "i2c_driver.h"

extern I2C_HandleTypeDef hi2c2;

static HTS221_Calib_t calib;

//-----------------------------------------------------
// 1) WHO_AM_I
//-----------------------------------------------------
HAL_StatusTypeDef HTS221_ReadWhoAmI(uint8_t *id)
{
    return I2C_ReadReg(HTS221_I2C_ADDR, HTS221_WHO_AM_I_REG, id);
}

//-----------------------------------------------------
// 2) INIT
//-----------------------------------------------------
HAL_StatusTypeDef HTS221_Init(void)
{
    HAL_StatusTypeDef status;

    uint8_t av_conf = 0b10100100;
    status = I2C_WriteReg(HTS221_I2C_ADDR, HTS221_AV_CONF, av_conf);
    if (status != HAL_OK) return status;

    uint8_t ctrl1 = (1 << 7) | (1 << 2) | (1 << 0);
    status = I2C_WriteReg(HTS221_I2C_ADDR, HTS221_CTRL_REG1, ctrl1);
    if (status != HAL_OK) return status;

    return HAL_OK;
}

//-----------------------------------------------------
// 3) CALIBRACIÓN
//-----------------------------------------------------
HAL_StatusTypeDef HTS221_ReadCalibration(void)
{
    uint8_t H0, H1;
    uint8_t T0_L, T1_L, T0T1_MSB;
    uint8_t buf[2];

    I2C_ReadReg(HTS221_I2C_ADDR, 0x30, &H0);
    I2C_ReadReg(HTS221_I2C_ADDR, 0x31, &H1);

    calib.H0_rH = H0 / 2.0f;
    calib.H1_rH = H1 / 2.0f;

    I2C_ReadMulti(HTS221_I2C_ADDR, 0x36 | HTS221_AUTO_INCREMENT, buf, 2);
    calib.H0_T0_OUT = (int16_t)(buf[1] << 8 | buf[0]);

    I2C_ReadMulti(HTS221_I2C_ADDR, 0x3A | HTS221_AUTO_INCREMENT, buf, 2);
    calib.H1_T0_OUT = (int16_t)(buf[1] << 8 | buf[0]);

    I2C_ReadReg(HTS221_I2C_ADDR, 0x32, &T0_L);
    I2C_ReadReg(HTS221_I2C_ADDR, 0x33, &T1_L);
    I2C_ReadReg(HTS221_I2C_ADDR, 0x35, &T0T1_MSB);

    uint8_t T0_M =  T0T1_MSB & 0x03;
    uint8_t T1_M = (T0T1_MSB & 0x0C) >> 2;

    calib.T0_degC = ((T0_M << 8) | T0_L) / 8.0f;
    calib.T1_degC = ((T1_M << 8) | T1_L) / 8.0f;

    I2C_ReadMulti(HTS221_I2C_ADDR, 0x3C | HTS221_AUTO_INCREMENT, buf, 2);
    calib.T0_OUT = (int16_t)(buf[1] << 8 | buf[0]);

    I2C_ReadMulti(HTS221_I2C_ADDR, 0x3E | HTS221_AUTO_INCREMENT, buf, 2);
    calib.T1_OUT = (int16_t)(buf[1] << 8 | buf[0]);

    return HAL_OK;
}

//-----------------------------------------------------
// 4) LECTURA RAW
//-----------------------------------------------------
HAL_StatusTypeDef HTS221_ReadRawHumidity(int16_t *raw)
{
    uint8_t buf[2];
    HAL_StatusTypeDef st = I2C_ReadMulti(HTS221_I2C_ADDR, HTS221_H_OUT_L | HTS221_AUTO_INCREMENT, buf, 2);
    if (st != HAL_OK) return st;

    *raw = (int16_t)((buf[1] << 8) | buf[0]);
    return HAL_OK;
}

HAL_StatusTypeDef HTS221_ReadRawTemperature(int16_t *raw)
{
    uint8_t buf[2];
    HAL_StatusTypeDef st = I2C_ReadMulti(HTS221_I2C_ADDR, HTS221_T_OUT_L | HTS221_AUTO_INCREMENT, buf, 2);
    if (st != HAL_OK) return st;

    *raw = (int16_t)((buf[1] << 8) | buf[0]);
    return HAL_OK;
}

//-----------------------------------------------------
// 5) CONVERSIÓN A °C y %RH
//-----------------------------------------------------
float HTS221_ComputeHumidity(int16_t raw)
{
    return calib.H0_rH +
           (raw - calib.H0_T0_OUT) *
           (calib.H1_rH - calib.H0_rH) /
           (calib.H1_T0_OUT - calib.H0_T0_OUT);
}

float HTS221_ComputeTemperature(int16_t raw)
{
    return calib.T0_degC +
           (raw - calib.T0_OUT) *
           (calib.T1_degC - calib.T0_degC) /
           (calib.T1_OUT - calib.T0_OUT);
}
