#include "drivers/PioData.h"
#include "ws2812.pio.h"

namespace ws2812 {
    void PioData::init_internal(uint pin) {
        gpio_init(pin);
        gpio_set_dir(pin, GPIO_OUT);

        uint offset = pio_add_program(pio, &ws2812_program);

        // period 1.25us
        // freq = 1s / 1.25us = 800000 Hz
        ws2812_program_init(pio, sm, offset, pin, 800000, false);
    }

    void PioData::put(uint32_t grb) {
        pio_sm_put_blocking(pio, sm, grb << 8u);
    }
}
