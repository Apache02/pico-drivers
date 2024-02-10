#ifndef PICO_DRIVERS_FPSMEASURE__H
#define PICO_DRIVERS_FPSMEASURE__H

#include <stdio.h>
#include "pico/time.h"

class FpsMeasure {
private:
    uint64_t start_ms = 0;
    uint32_t frame_count = 0;
    uint32_t fps = 0;

    inline static uint64_t now_ms() {
        return time_us_64() / 1000;
    }

public:
    FpsMeasure() {}

    void reset();

    void frame();

    int getFps();
};


#endif // PICO_DRIVERS_FPSMEASURE__H
