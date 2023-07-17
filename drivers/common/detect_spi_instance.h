#ifndef DRIVERS_COMMON_DETECT_SPI_INSTANCE__H
#define DRIVERS_COMMON_DETECT_SPI_INSTANCE__H

#include "hardware/spi.h"

#ifdef __cplusplus
extern "C" {
#endif

spi_inst_t *detect_spi_instance(uint mosi_pin, uint scl_pin);

#ifdef __cplusplus
};
#endif

#endif // DRIVERS_COMMON_DETECT_SPI_INSTANCE__H