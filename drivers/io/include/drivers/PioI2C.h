#ifndef PICO_DRIVERS_IO_PIO_I2C_H
#define PICO_DRIVERS_IO_PIO_I2C_H

#include "pico/stdlib.h"
#include "I2C.h"
#include "hardware/pio.h"


namespace IO {
    class PioI2C : public AbstractI2C {
    private:
        PIO pio;
        uint sm;
        uint sda;
        uint scl;

    public:
        PioI2C(PIO pio, uint sda, uint scl);

        void init(uint baudrate) override;

        int write(uint8_t addr, const uint8_t *src, size_t len, bool nostop) override;

        int read(uint8_t addr, uint8_t *src, size_t len, bool nostop) override;
    };
}


#endif // PICO_DRIVERS_IO_PIO_I2C_H
