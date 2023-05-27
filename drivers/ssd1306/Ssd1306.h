#ifndef DRIVERS_SSD1306__H
#define DRIVERS_SSD1306__H

#include "pico/stdlib.h"
#include "io.h"

#define SSD1306_DEFAULT_I2C_ADDRESS 0x3C

class Ssd1306 {
public:
    const uint8_t width;
    const uint8_t height;
    uint8_t *buffer = nullptr;
    size_t bufferSize = 0;
    DeviceIO *io;

private:

public:

    Ssd1306(DeviceIO *io, uint8_t width, uint8_t height);

    ~Ssd1306();

    void init();

    void update();
};


#endif // DRIVERS_SSD1306__H