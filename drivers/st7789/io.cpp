#include "drivers/st7789/io.h"

st7789::SPI::SPI(IO::SPI &spi, uint cs, uint dc, uint reset) : spi(spi), cs(cs), dc(dc), reset(reset) {
    init_io();
}

void st7789::SPI::init_io() {
    if (reset != -1) {
        gpio_init(reset);
        gpio_set_dir(reset, GPIO_OUT);
        gpio_put(reset, 0);
    }

    gpio_init(cs);
    gpio_set_dir(cs, GPIO_OUT);
    gpio_put(cs, 1);

    gpio_init(dc);
    gpio_set_dir(dc, GPIO_OUT);
    gpio_put(dc, 1);

    if (reset != -1) {
        gpio_put(reset, 1);
    }
}

void st7789::SPI::command(uint8_t reg) {
    gpio_put(dc, 0);
    gpio_put(cs, 0);
    spi.write(&reg, 1);
    gpio_put(cs, 1);
}

void st7789::SPI::command(uint8_t reg, const uint8_t *data, size_t length) {
    gpio_put(dc, 0);
    gpio_put(cs, 0);
    spi.write(&reg, 1);

    gpio_put(dc, 1);
    spi.write(data, length);

    gpio_put(cs, 1);
}

void st7789::SPI::command(uint8_t reg, const uint16_t *data, size_t length) {
    gpio_put(dc, 0);
    gpio_put(cs, 0);
    spi.write(&reg, 1);

    gpio_put(dc, 1);
    spi.write(data, length);

    gpio_put(cs, 1);
}

void st7789::SPI::write(const uint16_t *data, size_t length) {
    gpio_put(dc, 1);
    gpio_put(cs, 0);
    spi.write(data, length);
    gpio_put(cs, 1);
}
