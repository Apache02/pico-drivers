#include "drivers/detect_spi_instance.h"

// pin           bits     spi
// --------------------------
// 0        0 0 0 0 0       0
// 1        0 0 0 0 1       0
// 2        0 0 0 1 0       0
// 3        0 0 0 1 1       0
// 4        0 0 1 0 0       0
// 5        0 0 1 0 1       0
// 6        0 0 1 1 0       0
// 7        0 0 1 1 1       0
// 8        0 1 0 0 0       1
// 9        0 1 0 0 1       1
// 10       0 1 0 1 0       1
// 11       0 1 0 1 1       1
// 12       0 1 1 0 0       1
// 13       0 1 1 0 1       1
// 14       0 1 1 1 0       1
// 15       0 1 1 1 1       1
// 16       1 0 0 0 0       0
// 17       1 0 0 0 1       0
// 18       1 0 0 1 0       0
// 19       1 0 0 1 1       0

spi_inst_t *detect_spi_instance(uint mosi_pin, uint scl_pin) {
    if (mosi_pin > 19 || scl_pin > 19) {
        return nullptr;
    }

    const uint mask = 0b1000;
    if ((mosi_pin & mask) != (scl_pin & mask)) {
        return nullptr;
    }

    if (mosi_pin & mask) {
        return spi1;
    }

    return spi0;
}
