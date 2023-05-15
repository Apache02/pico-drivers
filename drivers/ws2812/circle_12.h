#ifndef DRIVERS_WS2812_CIRCLE_12__H
#define DRIVERS_WS2812_CIRCLE_12__H

#include "pico/stdlib.h"
#include "hardware/pio.h"

namespace ws2812 {
    class Circle12 {

    public:
        const size_t size = 12;

    private:
        uint pixels[12];
        PIO pio;
        uint sm; // state machine

        void init_internal(uint pin);

    public:
        Circle12(uint pin, PIO pio = pio0, uint sm = 0) : pio(pio), sm(sm) {
            init_internal(pin);
        };

        void setPixel(uint offset, uint32_t color);

        void fill(uint32_t color, bool showPixels = true);

        inline void clear() {
            fill(0, true);
        };

        void show(uint32_t shift = 0);
    };
}


#endif //DRIVERS_WS2812_CIRCLE_12__H
