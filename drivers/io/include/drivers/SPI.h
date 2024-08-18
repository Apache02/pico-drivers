#ifndef PICO_DRIVERS_IO_SPI_H
#define PICO_DRIVERS_IO_SPI_H

#include "hardware/spi.h"


namespace IO {
    class SPI {
    private:
        uint cs;
        uint miso;
        uint mosi;

    public:
        spi_inst_t *instance;

        SPI(spi_inst_t *instance) : instance(instance) {};

        SPI(uint mosi, uint miso, uint scl, uint cs);

        void init(uint baudrate);

        int write(const uint8_t *src, size_t len);
    };
}


#endif // PICO_DRIVERS_IO_SPI_H
