#ifndef DRIVERS_ST7789_IO__H
#define DRIVERS_ST7789_IO__H

#include "pico/stdlib.h"
#include "hardware/spi.h"
#include "../common/detect_spi_instance.h"

#ifndef ST7789_SPI_BAUD
#define ST7789_SPI_BAUD 62'500'000
#endif

class St7789_SPI {
private:
    uint sda; // mosi
    uint scl;
    uint cs;
    uint dc;
    uint reset;
    spi_inst_t *instance;

    bool mode16;

    void init_io(uint baudrate);

    void inline use_mode16() {
        if (!mode16) {
            spi_set_format(instance, 16, SPI_CPOL_0, SPI_CPHA_0, SPI_MSB_FIRST);
            mode16 = true;
        }
    }

    void inline use_mode8() {
        if (mode16) {
            spi_set_format(instance, 8, SPI_CPOL_0, SPI_CPHA_0, SPI_MSB_FIRST);
            mode16 = false;
        }
    }

public:
    St7789_SPI(
            uint sda,
            uint scl,
            uint cs,
            uint dc,
            uint reset = -1,
            uint baudrate = 62'500'000
    ) : sda(sda), scl(scl), cs(cs), dc(dc), reset(reset) {
        instance = detect_spi_instance(sda, scl);
        init_io(baudrate);
    }

    void command(uint8_t reg, const uint8_t *data = nullptr, size_t length = 0);

    void command(uint8_t reg, const uint16_t *data, size_t length);

    void write(const uint16_t *data, size_t length);
};

#endif // DRIVERS_ST7789_IO__H
