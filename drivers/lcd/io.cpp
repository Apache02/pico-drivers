
#include <drivers/lcd/display.h>

#include "drivers/lcd/io.h"

namespace LCD {
    void Direct::init_io() {
        uint control_pins_mask = (1 << rs) | (1 << en);
        gpio_init_mask(control_pins_mask);
        gpio_set_dir_out_masked(control_pins_mask);

        gpio_init_mask(data_pins_mask);
        gpio_set_dir_out_masked(data_pins_mask);
    }

    void Direct::command(const uint8_t byte) {
        gpio_put(rs, 0);
        gpio_put(en, 1);
        sleep_us(1);
        gpio_put_masked(data_pins_mask, ((uint32_t) byte) << db0);
        gpio_put(en, 0);
        sleep_us(1);
    }

    void Direct::write(const uint8_t byte) {
        gpio_put(rs, 1);
        gpio_put(en, 1);
        sleep_us(100);
        gpio_put_masked(data_pins_mask, ((uint32_t) byte) << db0);
        gpio_put(en, 0);
        sleep_us(100);
    }
}

