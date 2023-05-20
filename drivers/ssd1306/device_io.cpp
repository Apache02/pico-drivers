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

spi_inst_t *DeviceIO_SPI::detect_instance(uint sda_pin, uint scl_pin) {
    if (sda_pin > 7 && scl_pin > 7 && sda_pin < 16 && scl_pin < 16) {
        return spi1;
    }
    if ((sda_pin <= 7 || sda_pin >= 16) && (scl_pin <= 7 || scl_pin >= 16)) {
        return spi0;
    }
    return nullptr;
}

void DeviceIO_SPI::init_io(uint baudrate) {
    spi_init(instance, baudrate);

    gpio_set_function(sda, GPIO_FUNC_SPI);
    gpio_set_function(scl, GPIO_FUNC_SPI);
    if (cs != -1) {
        gpio_init(cs);
        gpio_set_dir(cs, GPIO_OUT);
        gpio_put(cs, 1);
    }
    gpio_init(dc);
    gpio_set_dir(dc, GPIO_OUT);
    gpio_put(dc, 0);
    if (reset != -1) {
        gpio_init(reset);
        gpio_set_dir(reset, GPIO_OUT);

        gpio_put(reset, 1);
        sleep_ms(1);
        gpio_put(reset, 0);
        sleep_ms(10);
        gpio_put(reset, 1);
    }
}

void DeviceIO_SPI::command(const uint8_t command) {
    write_internal(&command, 1, false);
}

void DeviceIO_SPI::commands(const uint8_t *commands, size_t length) {
    write_internal(commands, length, false);
}

void DeviceIO_SPI::write(const uint8_t *data, size_t length) {
    write_internal(data, length, true);
}

void DeviceIO_SPI::write_internal(const uint8_t *data, const size_t length, bool is_data) {
    gpio_put(dc, is_data);
    gpio_put(cs, 0);
    spi_write_blocking(instance, data, length);
    gpio_put(cs, 1);
}
