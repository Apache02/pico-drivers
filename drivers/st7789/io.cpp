#include "io.h"

void St7789_SPI::init_io(uint baudrate) {
    if (reset != -1) {
        gpio_init(reset);
        gpio_set_dir(reset, GPIO_OUT);
        gpio_put(reset, 0);
    }

    spi_init(instance, baudrate);
    spi_set_format(instance, 8, SPI_CPOL_0, SPI_CPHA_0, SPI_MSB_FIRST);
    mode16 = false;
    gpio_set_function(sda, GPIO_FUNC_SPI);
    gpio_set_function(scl, GPIO_FUNC_SPI);

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

void St7789_SPI::command(uint8_t reg, const uint8_t *data, size_t length) {
    use_mode8();

    gpio_put(dc, 0);
    gpio_put(cs, 0);
    spi_write_blocking(instance, &reg, 1);

    if (data) {
        gpio_put(dc, 1);
        spi_write_blocking(instance, data, length);
    }

    gpio_put(cs, 1);
}

void St7789_SPI::command(uint8_t reg, const uint16_t *data, size_t length) {
    use_mode8();

    gpio_put(dc, 0);
    gpio_put(cs, 0);
    spi_write_blocking(instance, &reg, 1);

    if (data) {
        gpio_put(dc, 1);
        use_mode16();
        spi_write16_blocking(instance, data, length);
    }

    gpio_put(cs, 1);
}

void St7789_SPI::write(const uint16_t *data, size_t length) {
    use_mode16();

    gpio_put(dc, 1);
    gpio_put(cs, 0);
    spi_write16_blocking(instance, data, length);
    gpio_put(cs, 1);
}
