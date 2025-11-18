/*
 * i2c_driver.c
 *
 *  Created on: Nov 17, 2025
 *      Author: admin
 */


#include "i2c_driver.h"

/* hi2c2 está definido en el código generado por CubeMX.
   No importa en qué archivo esté, lo declaramos como extern aquí. */
extern I2C_HandleTypeDef hi2c2;

HAL_StatusTypeDef I2C_WriteReg(uint8_t dev_addr,
                               uint8_t reg_addr,
                               uint8_t data)
{
    return HAL_I2C_Mem_Write(&hi2c2,
                             dev_addr,
                             reg_addr,
                             I2C_MEMADD_SIZE_8BIT,
                             &data,
                             1,
                             100);  // timeout 100 ms
}

HAL_StatusTypeDef I2C_ReadReg(uint8_t dev_addr,
                              uint8_t reg_addr,
                              uint8_t *data)
{
    return HAL_I2C_Mem_Read(&hi2c2,
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
    return HAL_I2C_Mem_Read(&hi2c2,
                            dev_addr,
                            reg_addr,
                            I2C_MEMADD_SIZE_8BIT,
                            buf,
                            len,
                            100);
}
