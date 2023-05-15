#ifndef DRIVERS_WS2812_COMMON__H
#define DRIVERS_WS2812_COMMON__H

namespace ws2812 {
    static inline uint32_t rgb_to_grb(uint32_t pixel_rgb) {
        return (((pixel_rgb >> 16) & 0xFF) << 8) // r
               | (((pixel_rgb >> 8) & 0xFF) << 16) // g
               | ((pixel_rgb >> 0) & 0xFF); // b
    }
}

#endif //DRIVERS_WS2812_COMMON__H
