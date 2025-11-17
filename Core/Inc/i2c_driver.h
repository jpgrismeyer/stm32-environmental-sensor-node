#ifndef I2C_DRIVER_H_
#define I2C_DRIVER_H_

#include "stm32l4xx_hal.h"
#include <stdint.h>

/**
 * @brief Inicializa el driver I2C con un handle ya configurado (por ejemplo hi2c2).
 *
 * Debe llamarse después de MX_I2C2_Init().
 */
HAL_StatusTypeDef I2C_Driver_Init(I2C_HandleTypeDef *hi2c);

/**
 * @brief Escribe un byte en un registro de un dispositivo I2C.
 *
 * @param dev_addr  Dirección I2C del dispositivo (7 bits << 1, estilo HAL).
 * @param reg_addr  Dirección del registro interno.
 * @param data      Byte a escribir.
 */
HAL_StatusTypeDef I2C_WriteReg(uint8_t dev_addr,
                               uint8_t reg_addr,
                               uint8_t data);

/**
 * @brief Lee un byte de un registro de un dispositivo I2C.
 */
HAL_StatusTypeDef I2C_ReadReg(uint8_t dev_addr,
                              uint8_t reg_addr,
                              uint8_t *data);

/**
 * @brief Lee varios bytes consecutivos a partir de un registro.
 */
HAL_StatusTypeDef I2C_ReadMulti(uint8_t dev_addr,
                                uint8_t reg_addr,
                                uint8_t *buf,
                                uint16_t len);

#endif /* I2C_DRIVER_H_ */
