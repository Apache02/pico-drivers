#include "Single.h"
#include "common.h"


namespace ws2812 {
    void Single::set(uint32_t rgb) {
        put(rgb_to_grb(rgb));
    }
}
