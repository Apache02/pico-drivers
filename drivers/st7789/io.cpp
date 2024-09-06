#include "drivers/st7789/io.h"
#include "hardware/spi.h"
#include <stdio.h>


st7789::IO_Spi::IO_Spi(IO::SPI &spi, uint cs, uint dc, uint reset) : spi(spi), cs(cs), dc(dc), reset(reset) {}

void st7789::IO_Spi::init() {
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

void st7789::IO_Spi::command(uint8_t reg) {
    gpio_put(dc, 0);
    gpio_put(cs, 0);
    spi.write(&reg, 1);
    gpio_put(cs, 1);
}

void st7789::IO_Spi::command(uint8_t reg, const uint8_t *data, size_t length) {
    gpio_put(dc, 0);
    gpio_put(cs, 0);
    spi.write(&reg, 1);

    gpio_put(dc, 1);
    spi.write(data, length);

    gpio_put(cs, 1);
}

void st7789::IO_Spi::command(uint8_t reg, const uint16_t *data, size_t length) {
    gpio_put(dc, 0);
    gpio_put(cs, 0);
    spi.write(&reg, 1);

    gpio_put(dc, 1);
    spi.write(data, length);

    gpio_put(cs, 1);
}

void st7789::IO_Spi::write(const uint16_t *data, size_t length) {
    gpio_put(dc, 1);
    gpio_put(cs, 0);
    spi.write(data, length);
    gpio_put(cs, 1);
}

st7789::IO_Spi_Dma::IO_Spi_Dma(IO::SPI &spi, uint cs, uint dc, uint reset) : IO_Spi(spi, cs, dc, reset) {}

st7789::IO_Spi_Dma::~IO_Spi_Dma() {
    if (dma_tx != -1) {
        dma_channel_unclaim(dma_tx);
        dma_tx = -1;
    }
}

void st7789::IO_Spi_Dma::init() {
    IO_Spi::init();

    dma_tx = dma_claim_unused_channel(false);
    if (dma_tx == -1) {
        return;
    }

    config = dma_channel_get_default_config(dma_tx);

    channel_config_set_transfer_data_size(&config, DMA_SIZE_16);
    channel_config_set_read_increment(&config, true);
    channel_config_set_write_increment(&config, false);
    channel_config_set_ring(&config, false, 0);
    if (spi.instance == spi0) {
        channel_config_set_dreq(&config, DREQ_SPI0_TX);
    } else {
        channel_config_set_dreq(&config, DREQ_SPI1_TX);
    }
}

void st7789::IO_Spi_Dma::command(uint8_t reg) {
    if (dma_tx == -1) {
        // fallback to regular SPI
        IO_Spi::command(reg);
        return;
    }

    wait_dma();

    IO_Spi::command(reg);
}

void st7789::IO_Spi_Dma::command(uint8_t reg, const uint8_t *data, size_t length) {
    if (dma_tx == -1) {
        IO_Spi::command(reg, data, length);
        return;
    }

    wait_dma();

    IO_Spi::command(reg, data, length);
}

void st7789::IO_Spi_Dma::command(uint8_t reg, const uint16_t *data, size_t length) {
    if (dma_tx == -1) {
        IO_Spi::command(reg, data, length);
        return;
    }

    wait_dma();

    if (length <= 8) {
        IO_Spi::command(reg, data, length);
        return;
    }

    gpio_put(dc, 0);
    gpio_put(cs, 0);
    spi.write(&reg, 1);

    gpio_put(dc, 1);
    spi.use_mode16();
    dma_channel_configure(dma_tx, &config, &spi_get_hw(spi.instance)->dr, data, length, true);
}

void st7789::IO_Spi_Dma::write(const uint16_t *data, size_t length) {
    if (dma_tx == -1) {
        IO_Spi::write(data, length);
        return;
    }

    wait_dma();

    IO_Spi::write(data, length);
}

void st7789::IO_Spi_Dma::wait_dma() {
    if (dma_channel_is_busy(dma_tx)) {
        dma_channel_wait_for_finish_blocking(dma_tx);
        // extra time for DMA to finish previous transfer
        sleep_us(3);
    }
}

