#ifndef DRIVERS_ST7789__H
#define DRIVERS_ST7789__H

#include "pico/stdlib.h"
#include "io.h"


class St7789 {
public:
    const uint16_t width;
    const uint16_t height;
    St7789_SPI *io;

private:
    void internal_init();

    void working_area(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2);

public:

    St7789(St7789_SPI *io, uint16_t width, uint16_t height) : io(io), width(width), height(height) {
        internal_init();
    }

    ~St7789();

    void fill(uint16_t color);

    void pixel(uint16_t x, uint16_t y, uint16_t color);

    void draw(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, const uint16_t * buffer, size_t length);
};


#endif // DRIVERS_ST7789__H