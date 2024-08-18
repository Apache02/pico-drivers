#ifndef PICO_DRIVERS_IO_I2C_H
#define PICO_DRIVERS_IO_I2C_H

#include "hardware/i2c.h"


namespace IO {
    class I2C {
    public:
        i2c_inst_t *instance;

        I2C(i2c_inst_t *instance) : instance(instance) {};

        I2C(uint sda, uint scl);

        void init(uint baudrate);

        int write(uint8_t addr, const uint8_t *src, size_t len, bool nostop);
    };
}


#endif // PICO_DRIVERS_IO_I2C_H
