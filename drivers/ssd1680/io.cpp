#include "io.h"

spi_inst_t *Ssd1680_IO::detect_instance(uint sda_pin, uint scl_pin) {
    if (sda_pin > 7 && scl_pin > 7 && sda_pin < 16 && scl_pin < 16) {
        return spi1;
    }
    if ((sda_pin <= 7 || sda_pin >= 16) && (scl_pin <= 7 || scl_pin >= 16)) {
        return spi0;
    }
    return nullptr;
}

void Ssd1680_IO::init_io(uint baudrate) {
    spi_set_format(instance, 8, SPI_CPOL_0, SPI_CPHA_0, SPI_MSB_FIRST);
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
    if (busy != -1) {
        gpio_init(busy);
        gpio_set_dir(reset, GPIO_IN);
    }
    if (reset != -1) {
        gpio_init(reset);
        gpio_set_dir(reset, GPIO_OUT);

        gpio_put(reset, 1);
        sleep_ms(100);
        gpio_put(reset, 0);
        sleep_ms(100);
        gpio_put(reset, 1);
    }

    wait_busy();
}

bool Ssd1680_IO::is_busy() {
    if (busy == -1) {
        return false;
    }
    return gpio_get(busy);
}

void Ssd1680_IO::wait_busy() {
    while (is_busy()) {
        asm volatile ("nop; nop; nop; nop; nop; nop; nop; nop;");
    }
}

void Ssd1680_IO::command(uint8_t reg, const uint8_t *data, size_t length) {
    gpio_put(dc, 0);
    gpio_put(cs, 0);
    spi_write_blocking(instance, &reg, 1);

    if (data) {
        gpio_put(dc, 1);
        spi_write_blocking(instance, data, length);
    }

    gpio_put(cs, 1);
}

