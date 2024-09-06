#ifndef DRIVERS_ST7789_IO__H
#define DRIVERS_ST7789_IO__H

#include "pico/stdlib.h"
#include "hardware/spi.h"
#include "hardware/dma.h"
#include "drivers/SPI.h"
#include "drivers/detect_spi_instance.h"

#ifndef ST7789_SPI_BAUD
#define ST7789_SPI_BAUD 62'500'000
#endif

namespace st7789 {

    class IO_Spi {
    protected:
        IO::SPI &spi;
        uint cs;
        uint dc;
        uint reset;

    public:
        IO_Spi() = delete;

        IO_Spi(IO::SPI &spi, uint cs, uint dc, uint reset = -1);

        void init();

        virtual void command(uint8_t reg);

        virtual void command(uint8_t reg, const uint8_t *data, size_t length);

        virtual void command(uint8_t reg, const uint16_t *data, size_t length);

        virtual void write(const uint16_t *data, size_t length);
    };

    class IO_Spi_Dma : public IO_Spi {
    private:
        uint dma_tx = -1;
        dma_channel_config config;

        void wait_dma();

    public:
        IO_Spi_Dma() = delete;

        IO_Spi_Dma(IO::SPI &spi, uint cs, uint dc, uint reset = -1);

        ~IO_Spi_Dma();

        void init();

        void command(uint8_t reg) override;

        void command(uint8_t reg, const uint8_t *data, size_t length) override;

        void command(uint8_t reg, const uint16_t *data, size_t length) override;

        void write(const uint16_t *data, size_t length) override;
    };
}

#endif // DRIVERS_ST7789_IO__H
