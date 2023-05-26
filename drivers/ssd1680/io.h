#ifndef DRIVERS_SSD1680_IO__H
#define DRIVERS_SSD1680_IO__H

#include "pico/stdlib.h"
#include "hardware/spi.h"


class Ssd1680_IO {
private:
    uint sda; // mosi
    uint scl;
    uint cs;
    uint dc;
    uint reset;
    uint busy;
    spi_inst_t *instance;

    spi_inst_t *detect_instance(uint sda, uint scl);

    void init_io(uint baudrate);

public:
    Ssd1680_IO(
            uint sda,
            uint scl,
            uint cs,
            uint dc,
            uint reset = -1,
            uint busy = -1,
            uint baudrate = 1'000'000
    ) : sda(sda), scl(scl), cs(cs), dc(dc), reset(reset), busy(busy) {
        instance = detect_instance(sda, scl);
        init_io(baudrate);
    }

    void command(uint8_t reg, const uint8_t *data = nullptr, size_t length = 0);

    bool is_busy();

    void wait_busy();
};

#endif //DRIVERS_SSD1680_IO__H
