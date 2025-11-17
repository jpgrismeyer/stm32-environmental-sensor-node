/*
 * i2c_driver.c
 *
 *  Created on: Nov 17, 2025
 *      Author: admin
 */


#include "i2c_driver.h"

/* Handle interno que usará el driver */
static I2C_HandleTypeDef *i2c_handle = NULL;

HAL_StatusTypeDef I2C_Driver_Init(I2C_HandleTypeDef *hi2c)
{
    if (hi2c == NULL) {
        return HAL_ERROR;
    }

    i2c_handle = hi2c;
    return HAL_OK;
}

HAL_StatusTypeDef I2C_WriteReg(uint8_t dev_addr,
                               uint8_t reg_addr,
                               uint8_t data)
{
    if (i2c_handle == NULL) {
        return HAL_ERROR;
    }

    return HAL_I2C_Mem_Write(i2c_handle,
                             dev_addr,
                             reg_addr,
                             I2C_MEMADD_SIZE_8BIT,
                             &data,
                             1,
                             100);  // timeout en ms
}

HAL_StatusTypeDef I2C_ReadReg(uint8_t dev_addr,
                              uint8_t reg_addr,
                              uint8_t *data)
{
    if (i2c_handle == NULL) {
        return HAL_ERROR;
    }

    return HAL_I2C_Mem_Read(i2c_handle,
                            dev_addr,
                            reg_addr,
                            I2C_MEMADD_SIZE_8BIT,
                            data,
                            1,
                            100);
}

HAL_StatusTypeDef I2C_ReadMulti(uint8_t dev_addr,
                                uint8_t reg_addr,
                                uint8_t *buf,
                                uint16_t len)
{
    if (i2c_handle == NULL) {
        return HAL_ERROR;
    }

    return HAL_I2C_Mem_Read(i2c_handle,
                            dev_addr,
                            reg_addr,
                            I2C_MEMADD_SIZE_8BIT,
                            buf,
                            len,
                            100);
}
