/*
 * WeAct Studio e-paper e-inc display
 * 296x128 pixels, black, white and red colors
 * Long time to update
 */
#ifndef DRIVERS_SSD1680__H
#define DRIVERS_SSD1680__H

#include "pico/stdlib.h"
#include "io.h"


class Ssd1680 {
public:
    const uint16_t width;
    const uint16_t height;
    uint8_t *buffer_mono = nullptr;
    uint8_t *buffer_color = nullptr;
    size_t bufferSize = 0;
    Ssd1680_IO *io;

private:

public:

    Ssd1680(Ssd1680_IO *io, uint16_t width, uint16_t height) : io(io), width(width), height(height) {}

    ~Ssd1680();

    void init();

    void update();
};


#endif // DRIVERS_SSD1680__H