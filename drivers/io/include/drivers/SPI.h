#ifndef PICO_DRIVERS_IO_SPI_H
#define PICO_DRIVERS_IO_SPI_H

#include "hardware/spi.h"


namespace IO {
    // 62.5 MHz
    const uint SPI_BAUDRATE_MAX = 62.5 * 1'000'000;

    class SPI {
    private:
        bool mode16;

        inline void use_mode16();

        inline void use_mode8();

    public:
        spi_inst_t *instance;

        SPI(spi_inst_t *instance);

        SPI(uint mosi, uint miso, uint scl);

        virtual void init(uint baudrate);

        virtual int write(const uint8_t *data, size_t len);

        virtual int write(const uint16_t *data, size_t len);
    };
}


#endif // PICO_DRIVERS_IO_SPI_H
