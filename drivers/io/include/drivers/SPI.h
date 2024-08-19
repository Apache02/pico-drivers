#ifndef PICO_DRIVERS_IO_SPI_H
#define PICO_DRIVERS_IO_SPI_H

#include "hardware/spi.h"


namespace IO {
    const uint SPI_BAUDRATE_MAX = 62.5 * 1'000'000;

    class SPI {
    public:
        spi_inst_t *instance;

        SPI(spi_inst_t *instance);

        SPI(uint mosi, uint miso, uint scl);

        void init(uint baudrate);

        int write(const uint8_t *src, size_t len);
    };
}


#endif // PICO_DRIVERS_IO_SPI_H
