#ifndef DRIVERS_WS2812_PIO__H
#define DRIVERS_WS2812_PIO__H

#include "pico/stdlib.h"
#include "hardware/pio.h"

namespace ws2812 {
    class PioData {

    private:
        PIO pio;
        uint sm; // state machine

        void init_internal(uint pin);

    protected:
        PioData(uint pin, PIO pio = pio0, uint sm = 0) : pio(pio), sm(sm) {
            init_internal(pin);
        };

        void put(uint32_t grb);
    };
}

#endif //DRIVERS_WS2812_PIO__H
