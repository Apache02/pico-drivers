#include "device_io.h"
#include <string.h>

#define CHUNK_SIZE 64

i2c_inst_t *DeviceIO_I2C::detect_instance(uint pin_sda, uint pin_scl) {
    invalid_params_if(I2C, (pin_sda & 2) != (pin_scl & 2));
    return i2c_get_instance((pin_sda >> 1) & 1);
}

void DeviceIO_I2C::init_io(uint baudrate) {
    gpio_set_function(sda, GPIO_FUNC_I2C);
    gpio_set_function(scl, GPIO_FUNC_I2C);
    i2c_init(instance, baudrate);
}

void DeviceIO_I2C::command(const uint8_t command) {
    const uint8_t message[2] = {0x00, command};
    i2c_write_blocking(instance, address, message, 2, false);
}

void DeviceIO_I2C::commands(const uint8_t *commands, size_t length) {
    chunked_write(0x00, commands, length);
}

void DeviceIO_I2C::write(const uint8_t *data, size_t length) {
    chunked_write(0x40, data, length);
}

void DeviceIO_I2C::chunked_write(const uint8_t control_byte, const uint8_t *data, const size_t length) {
    uint8_t message[CHUNK_SIZE + 1] = {};

    for (size_t left_length = length, chunk_length = sizeof(message) - 1; left_length > 0; left_length -= chunk_length) {
        if (left_length < chunk_length) {
            chunk_length = left_length;
        }
        memcpy(&message[1], &data[length - left_length], chunk_length);
        message[0] = control_byte;
        i2c_write_blocking(instance, address, message, chunk_length + 1, left_length > chunk_length);
    }
}
