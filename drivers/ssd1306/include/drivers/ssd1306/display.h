#ifndef DRIVERS_SSD1306__H
#define DRIVERS_SSD1306__H

#include "pico/stdlib.h"
#include "io.h"

#define SSD1306_DEFAULT_I2C_ADDRESS 0x3C

namespace ssd1306 {

    class Display {
    public:
        const uint8_t width;
        const uint8_t height;
        uint8_t *buffer = nullptr;
        size_t bufferSize = 0;
        DeviceIO *io;

    private:

    public:

        Display(DeviceIO *io, uint8_t width, uint8_t height);

        ~Display();

        void init();

        void update();
    };

}

#endif // DRIVERS_SSD1306__H