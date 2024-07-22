#ifndef DRIVERS_LCD_DEVICE_IO__H
#define DRIVERS_LCD_DEVICE_IO__H

#include <stdlib.h>
#include "pico/stdlib.h"
#include "hardware/gpio.h"

namespace LCD {

    class DeviceIO {
    public:
        virtual void command(const uint8_t byte) {};

        virtual void write(const uint8_t byte) {};
    };


    class Direct : public DeviceIO {
    private:
        uint rs;
        uint en;
        uint db0; // first pin
        uint32_t data_pins_mask;

        void init_io();

    public:
        Direct(uint rs_pin, uint enable_pin, uint db0_pin) : rs(rs_pin), en(enable_pin), db0(db0_pin) {
            data_pins_mask = (1 << (db0 + 0))
                             | (1 << (db0 + 1))
                             | (1 << (db0 + 2))
                             | (1 << (db0 + 3))
                             | (1 << (db0 + 4))
                             | (1 << (db0 + 5))
                             | (1 << (db0 + 6))
                             | (1 << (db0 + 7));
            init_io();
        }

        void command(const uint8_t byte) override;
        void write(const uint8_t byte) override;
    };
}

#endif // DRIVERS_LCD_DEVICE_IO__H