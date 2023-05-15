#ifndef DRIVERS_WS2812_SINGLE__H
#define DRIVERS_WS2812_SINGLE__H

#include "pico/stdlib.h"
#include "PioData.h"

namespace ws2812 {
    class Single: PioData {

    public:
        Single(uint pin, PIO pio = pio0, uint sm = 0) : PioData(pin, pio, sm) {};

        void set(uint32_t rgb);
    };
}


#endif //DRIVERS_WS2812_SINGLE__H
