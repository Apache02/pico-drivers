#ifndef DRIVERS_LCD_DEVICE_IO__H
#define DRIVERS_LCD_DEVICE_IO__H

#include <stdlib.h>
#include "pico/stdlib.h"
#include "hardware/gpio.h"

namespace LCD {

    class DeviceIO {
    public:
        virtual void configure() = 0;

        virtual void command(uint8_t byte) = 0;

        virtual void write(uint8_t byte) = 0;
    };


    // 8 bits direct connection
    class Direct : public DeviceIO {
    private:
        uint rs;
        uint en;
        uint db0; // first pin
        uint32_t data_pins_mask;

        void put8bits(uint8_t bits);

    public:
        Direct(uint rs_pin, uint enable_pin, uint db0_pin);

        void configure() override;

        void command(uint8_t byte) override;

        void write(uint8_t byte) override;
    };


    // 4 bits direct connection
    class Direct4Bit : public DeviceIO {
    private:
        uint rs;
        uint en;
        uint db4; // d4 pin
        uint32_t data_pins_mask;

        void put4bits(uint8_t bits);

    public:
        Direct4Bit(uint rs_pin, uint enable_pin, uint db4_pin);

        void configure() override;

        void command(uint8_t byte) override;

        void write(uint8_t byte) override;

    };
}

#endif // DRIVERS_LCD_DEVICE_IO__H