#ifndef DRIVERS_DISPLAY_IO__H
#define DRIVERS_DISPLAY_IO__H

#include <stdlib.h>
#include "pico/stdlib.h"
#include "hardware/gpio.h"
#include "hardware/i2c.h"
#include "hardware/spi.h"


class DeviceIO {
public:
    virtual void command(const uint8_t command) {};

    virtual void commands(const uint8_t *commands, size_t length) {};

    virtual void write(const uint8_t *data, size_t length) {};
};


class DeviceIO_I2C : public DeviceIO {
private:
    uint sda;
    uint scl;
    i2c_inst_t *instance;
    uint8_t address;

    i2c_inst_t *detect_instance(uint sda, uint scl);

    void init_io(uint baudrate);

    void chunked_write(const uint8_t control_byte, const uint8_t * data, const size_t length);

public:
    DeviceIO_I2C(
            uint sda,
            uint scl,
            i2c_inst_t *instance,
            uint8_t address,
            uint baudrate = 400000
    ) : sda(sda), scl(scl), instance(instance), address(address) {
        init_io(baudrate);
    }

    DeviceIO_I2C(
            uint sda,
            uint scl,
            uint8_t address,
            uint baudrate = 400000
    ) : sda(sda), scl(scl), address(address) {
        instance = detect_instance(sda, scl);
        init_io(baudrate);
    }

    void command(const uint8_t command) override;

    void commands(const uint8_t *commands, size_t length) override;

    void write(const uint8_t *data, size_t length) override;
};

#endif // DRIVERS_DISPLAY_IO__H