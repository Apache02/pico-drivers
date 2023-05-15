#include "circle_12.h"
#include "ws2812.pio.h"
#include "common.h"

namespace ws2812 {

    void Circle12::init_internal(uint pin) {
        gpio_init(pin);
        gpio_set_dir(pin, GPIO_OUT);

        uint offset = pio_add_program(pio, &ws2812_program);

        // period 1.25us
        // freq = 1s / 1.25us = 800000 Hz
        ws2812_program_init(pio, sm, offset, pin, 800000, false);
    }

    void Circle12::show(uint32_t shift) {
        for (auto i = 0; i < size; i++) {
            pio_sm_put_blocking(pio, sm, pixels[(i + (size - (shift % size))) % size] << 8u);
        }
    }

    void Circle12::fill(uint32_t color, bool showPixels) {
        for (auto i = 0; i < size; i++) {
            setPixel(i, color);
        }
        if (showPixels) {
            show();
        }
    }

    void Circle12::setPixel(uint offset, uint32_t rgb) {
        if (offset >= size) {
            return;
        }

        pixels[offset] = rgb_to_grb(rgb);
    }
}
