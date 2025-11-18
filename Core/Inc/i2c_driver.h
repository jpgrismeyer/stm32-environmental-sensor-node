#ifndef I2C_DRIVER_H_
#define I2C_DRIVER_H_

#include "stm32l4xx_hal.h"
#include <stdint.h>

/**
 * Escribe un byte en un registro de un dispositivo I2C.
 */
HAL_StatusTypeDef I2C_WriteReg(uint8_t dev_addr,
                               uint8_t reg_addr,
                               uint8_t data);

/**
 * Lee un byte de un registro de un dispositivo I2C.
 */
HAL_StatusTypeDef I2C_ReadReg(uint8_t dev_addr,
                              uint8_t reg_addr,
                              uint8_t *data);

/**
 * Lee varios bytes consecutivos.
 */
HAL_StatusTypeDef I2C_ReadMulti(uint8_t dev_addr,
                                uint8_t reg_addr,
                                uint8_t *buf,
                                uint16_t len);

#endif /* I2C_DRIVER_H_ */

