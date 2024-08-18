#include "include/drivers/I2C.h"
#include "hardware/gpio.h"


IO::I2C::I2C(uint sda, uint scl) {
    invalid_params_if(I2C, (sda & 2) != (scl & 2));
    instance = i2c_get_instance((sda >> 1) & 1);

    gpio_set_function(sda, GPIO_FUNC_I2C);
    gpio_set_function(scl, GPIO_FUNC_I2C);
}

void IO::I2C::init(uint baudrate) {

    i2c_init(instance, baudrate);
}

int IO::I2C::write(uint8_t addr, const uint8_t *src, size_t len, bool nostop) {
    return i2c_write_blocking(instance, addr, src, len, nostop);
}

