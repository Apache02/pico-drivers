#include "include/drivers/SPI.h"
#include "hardware/gpio.h"

static spi_inst_t *detect_instance(uint rx_or_tx1, uint rx_or_tx2, uint scl) {
    if (
            (scl == -1)
            || (scl > 19)
            || (rx_or_tx1 == -1 && rx_or_tx2 == -1)
            || (rx_or_tx1 != -1 && rx_or_tx1 > 19)
            || (rx_or_tx2 != -1 && rx_or_tx2 > 19)
            || (rx_or_tx1 != -1 && (rx_or_tx1 & 0b1000) != (0b1000 & scl))
            || (rx_or_tx2 != -1 && (rx_or_tx2 & 0b1000) != (0b1000 & scl))
            ) {
        return nullptr;
    }

    return 0b1000 & scl ? spi1 : spi0;
}

IO::SPI::SPI(spi_inst_t *instance) : instance(instance) {}

IO::SPI::SPI(uint mosi, uint miso, uint scl) {

    instance = detect_instance(mosi, miso, scl);
    assert(instance != nullptr);

    gpio_set_function(scl, GPIO_FUNC_SPI);
    if (mosi != -1) gpio_set_function(mosi, GPIO_FUNC_SPI);
    if (miso != -1) gpio_set_function(miso, GPIO_FUNC_SPI);
}

void IO::SPI::use_mode16() {
    if (!mode16) {
        spi_set_format(instance, 16, format.cpol, format.cpha, format.order);
        mode16 = true;
    }
}

void IO::SPI::use_mode8() {
    if (mode16) {
        spi_set_format(instance, 8, format.cpol, format.cpha, format.order);
        mode16 = false;
    }
}

void IO::SPI::init(uint baudrate, spi_cpol_t cpol, spi_cpha_t cpha, spi_order_t order) {
    format = {
            cpol,
            cpha,
            order
    };
    spi_init(instance, baudrate);
    spi_set_format(instance, 8, format.cpol, format.cpha, format.order);
    mode16 = false;
}

int IO::SPI::write(const uint8_t *data, size_t len) {
    use_mode8();
    return spi_write_blocking(instance, data, len);
}

int IO::SPI::write(const uint16_t *data, size_t len) {
    use_mode16();
    return spi_write16_blocking(instance, data, len);
}
