#include "Circle12.h"
#include "common.h"

namespace ws2812 {
    void Circle12::show(uint32_t shift) {
        for (auto i = 0; i < size; i++) {
            put(pixels[(i + (size - (shift % size))) % size]);
        }
    }

    void Circle12::fill(uint32_t color, bool showPixels) {
        for (auto i = 0; i < size; i++) {
            setPixel(i, color);
        }
        if (showPixels) {
            show();
        }
    }

    void Circle12::setPixel(uint offset, uint32_t rgb) {
        if (offset >= size) {
            return;
        }

        pixels[offset] = rgb_to_grb(rgb);
    }
}
