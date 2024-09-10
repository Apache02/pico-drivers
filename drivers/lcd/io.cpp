#include "drivers/lcd/io.h"

namespace LCD {

    Direct::Direct(uint rs_pin, uint enable_pin, uint db0_pin) : rs(rs_pin), en(enable_pin), db0(db0_pin) {
        data_pins_mask = (1 << (db0 + 0))
                         | (1 << (db0 + 1))
                         | (1 << (db0 + 2))
                         | (1 << (db0 + 3))
                         | (1 << (db0 + 4))
                         | (1 << (db0 + 5))
                         | (1 << (db0 + 6))
                         | (1 << (db0 + 7));

        uint control_pins_mask = (1 << rs) | (1 << en);
        gpio_init_mask(control_pins_mask);
        gpio_set_dir_out_masked(control_pins_mask);

        gpio_init_mask(data_pins_mask);
        gpio_set_dir_out_masked(data_pins_mask);
    }

    void Direct::configure() {
        gpio_put(rs, 0);
        put8bits(0b00110000);
        sleep_us(4500);
        put8bits(0b00110000);
        sleep_us(100);
        put8bits(0b00110000);
        put8bits(0b00100000);
        sleep_us(100);
    }

    void Direct::put8bits(uint8_t bits) {
        gpio_put(en, 1);
        sleep_us(1);
        gpio_put_masked(data_pins_mask, ((uint32_t) bits) << db0);
        gpio_put(en, 0);
        sleep_us(1);
    }

    void Direct::command(uint8_t byte) {
        if ((byte & 0b11100000) == 0b00100000) {
            byte |= 0b00010000; // force 8 bits mode
        }
        gpio_put(rs, 0);
        put8bits(byte);
    }

    void Direct::write(uint8_t byte) {
        gpio_put(rs, 1);
        gpio_put(en, 1);
        sleep_us(100);
        gpio_put_masked(data_pins_mask, ((uint32_t) byte) << db0);
        gpio_put(en, 0);
        sleep_us(100);
    }

    Direct4Bit::Direct4Bit(uint rs_pin, uint enable_pin, uint db4_pin) : rs(rs_pin), en(enable_pin), db4(db4_pin) {
        data_pins_mask = (1 << (db4 + 0))
                         | (1 << (db4 + 1))
                         | (1 << (db4 + 2))
                         | (1 << (db4 + 3));

        uint control_pins_mask = (1 << rs) | (1 << en);
        gpio_init_mask(control_pins_mask);
        gpio_set_dir_out_masked(control_pins_mask);

        gpio_init_mask(data_pins_mask);
        gpio_set_dir_out_masked(data_pins_mask);
    }

    void Direct4Bit::configure() {
        gpio_put(rs, 0);
        put4bits(0b0011);
        sleep_us(4500);
        put4bits(0b0011);
        sleep_us(100);
        put4bits(0b0011);
        put4bits(0b0010);
        sleep_us(100);
    }

    void Direct4Bit::put4bits(uint8_t bits) {
        gpio_put_masked(data_pins_mask, ((uint32_t) (bits & 0b1111)) << db4);
        sleep_us(1);
        gpio_put(en, 1);
        sleep_us(1);
        gpio_put(en, 0);
        sleep_us(1);
    }

    void Direct4Bit::command(uint8_t byte) {
        if ((byte & 0b11100000) == 0b00100000) {
            byte &= ~0b00010000; // force 4 bits mode
        }

        uint8_t high = ((byte >> 4) & 0x0F);
        uint8_t low = ((byte >> 0) & 0x0F);

        gpio_put(rs, 0);
        put4bits(high);
        put4bits(low);

        sleep_us(100);
    }

    void Direct4Bit::write(uint8_t byte) {
        uint8_t high = ((byte >> 4) & 0x0F);
        uint8_t low = ((byte >> 0) & 0x0F);

        gpio_put(rs, 1);
        put4bits(high);
        put4bits(low);

        sleep_us(100);
    }
}

