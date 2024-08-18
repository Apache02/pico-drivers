#include "drivers/ssd1306/io.h"
#include <string.h>


#define CHUNK_SIZE 64


void ssd1306::I2C::command(const uint8_t command) {
    const uint8_t message[2] = {0x00, command};
    iic.write(address, message, 2, false);
}

void ssd1306::I2C::commands(const uint8_t *commands, size_t length) {
    chunked_write(0x00, commands, length);
}

void ssd1306::I2C::write(const uint8_t *data, size_t length) {
    chunked_write(0x40, data, length);
}

void ssd1306::I2C::chunked_write(const uint8_t control_byte, const uint8_t *data, const size_t length) {
    uint8_t message[CHUNK_SIZE + 1] = {};

    for (size_t left_length = length, chunk_length = sizeof(message) - 1;
         left_length > 0; left_length -= chunk_length) {
        if (left_length < chunk_length) {
            chunk_length = left_length;
        }
        memcpy(&message[1], &data[length - left_length], chunk_length);
        message[0] = control_byte;
        iic.write(address, message, chunk_length + 1, left_length > chunk_length);
    }
}


ssd1306::SPI::SPI(IO::SPI &spi, uint dc, uint reset) : spi(spi), dc(dc) {
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

void ssd1306::SPI::command(const uint8_t command) {
    gpio_put(dc, 0);
    spi.write(&command, 1);
}

void ssd1306::SPI::commands(const uint8_t *commands, size_t length) {
    gpio_put(dc, 0);
    spi.write(commands, length);
}

void ssd1306::SPI::write(const uint8_t *data, size_t length) {
    gpio_put(dc, 1);
    spi.write(data, length);
}
