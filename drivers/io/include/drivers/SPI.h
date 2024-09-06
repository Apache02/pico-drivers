#ifndef PICO_DRIVERS_IO_SPI_H
#define PICO_DRIVERS_IO_SPI_H

#include "hardware/spi.h"


namespace IO {
    // 62.5 MHz
    const uint SPI_BAUDRATE_MAX = 62.5 * 1'000'000;

    class SPI {
    private:
        bool mode16;
        struct {
            spi_cpol_t cpol;
            spi_cpha_t cpha;
            spi_order_t order;
        } format;

    public:
        spi_inst_t *instance;

        SPI(spi_inst_t *instance);

        SPI(uint mosi, uint miso, uint scl);

        void init(uint baudrate, spi_cpol_t cpol = SPI_CPOL_0, spi_cpha_t cpha = SPI_CPHA_0, spi_order_t order = SPI_MSB_FIRST);

        int write(const uint8_t *data, size_t len);

        int write(const uint16_t *data, size_t len);

        void use_mode8();

        void use_mode16();
    };
}


#endif // PICO_DRIVERS_IO_SPI_H
