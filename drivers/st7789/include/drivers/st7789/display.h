#ifndef DRIVERS_ST7789__H
#define DRIVERS_ST7789__H

#include "pico/stdlib.h"
#include "drivers/st7789/io.h"

namespace st7789 {
    class Display {
    public:
        const uint16_t width;
        const uint16_t height;
        SPI *io;

    private:
        void internal_init();

        void working_area(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2);

    public:

        Display(SPI *io, uint16_t width, uint16_t height) : io(io), width(width), height(height) {
            internal_init();
        }

        ~Display();

        void fill(uint16_t color);

        void pixel(uint16_t x, uint16_t y, uint16_t color);

        void draw(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, const uint16_t *buffer, size_t length);
    };
}

#endif // DRIVERS_ST7789__H