#ifndef DRIVERS_ST7789_IO__H
#define DRIVERS_ST7789_IO__H

#include "pico/stdlib.h"
#include "hardware/spi.h"
#include "drivers/SPI.h"
#include "drivers/detect_spi_instance.h"

#ifndef ST7789_SPI_BAUD
#define ST7789_SPI_BAUD 62'500'000
#endif

namespace st7789 {

    class SPI {
    private:
        IO::SPI &spi;
        uint cs;
        uint dc;
        uint reset;

        void init_io();

    public:
        SPI(IO::SPI &spi, uint cs, uint dc, uint reset = -1);

        void command(uint8_t reg);

        void command(uint8_t reg, const uint8_t *data, size_t length);

        void command(uint8_t reg, const uint16_t *data, size_t length);

        void write(const uint16_t *data, size_t length);
    };

}

#endif // DRIVERS_ST7789_IO__H
