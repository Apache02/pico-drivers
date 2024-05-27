/*
 * WeAct Studio e-paper e-inc display
 * 296x128 pixels, black, white and red colors
 * Long time to update
 */
#ifndef DRIVERS_SSD1680__H
#define DRIVERS_SSD1680__H

#include "pico/stdlib.h"
#include "io.h"

namespace ssd1680 {

    class Display {
    public:
        const uint16_t width;
        const uint16_t height;
        uint8_t *buffer_mono = nullptr;
        uint8_t *buffer_color = nullptr;
        size_t bufferSize = 0;
        SPI *io;

    private:

    public:

        Display(SPI *io, uint16_t width, uint16_t height) : io(io), width(width), height(height) {}

        ~Display();

        void init();

        void update();
    };

}

#endif // DRIVERS_SSD1680__H