#ifndef PICO_DRIVERS_IO_I2C_H
#define PICO_DRIVERS_IO_I2C_H

#include "hardware/i2c.h"


namespace IO {
    class AbstractI2C {
    public:
        virtual void init(uint baudrate) = 0;

        virtual int write(uint8_t addr, const uint8_t *src, size_t len, bool nostop) = 0;
    };


    class I2C : public AbstractI2C {
    public:
        i2c_inst_t *instance;

        I2C(i2c_inst_t *instance) : instance(instance) {};

        I2C(uint sda, uint scl);

        void init(uint baudrate) override;

        int write(uint8_t addr, const uint8_t *src, size_t len, bool nostop) override;
    };
}


#endif // PICO_DRIVERS_IO_I2C_H
