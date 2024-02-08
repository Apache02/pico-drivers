#include <stdio.h>
#include "pico/stdlib.h"
#include "pico/time.h"
#include "pico/binary_info.h"
#include "drivers/led/led.h"
#include "drivers/ws2812/Circle12.h"

// board config
#define LED_PIN PICO_DEFAULT_LED_PIN
#define WS2812_DATA_PIN 0

// global variables
LED led(LED_PIN);
ws2812::Circle12 circle(WS2812_DATA_PIN, pio0, 0);


void startup_test() {
    printf("%s START\n", __FUNCTION__);
    led.on();
    circle.fill(0x0F0000, true);
    sleep_ms(300);
    circle.fill(0x000F00, true);
    sleep_ms(300);
    circle.fill(0x00000F, true);
    sleep_ms(300);
    circle.clear();
    sleep_ms(1100);
    led.off();
    sleep_ms(10);
    printf("%s DONE\n", __FUNCTION__);
}

static inline uint32_t rgb_brightness(const uint32_t color, float brightness) {
    uint8_t r = (color >> (8 * 2)) & 0xFF;
    uint8_t g = (color >> (8 * 1)) & 0xFF;
    uint8_t b = (color >> (8 * 0)) & 0xFF;
    r = r * brightness;
    g = g * brightness;
    b = b * brightness;
    return (r << (8 * 2)) | (g << (8 * 1)) | (b << (8 * 0));
}

int main() {
    bi_decl(bi_program_description("This is a lib-drivers-example binary."));
    bi_decl(bi_1pin_with_name(LED_PIN, "On-board LED"));

    stdio_init_all();

    startup_test();

    static const uint32_t colors[] = {
            0x4F0000,
            0x004F00,
            0x00004F,
            0x4F4F00,
            0x004F4F,
            0x4F004F,
            0x2F4F00,
            0x002F4F,
            0x4F002F,
            0x004F2F,
            0x2F004F,
            0x4F2F00,
            0x4F4F4F,
    };

    for (auto i = 0;; i++) {
        for (auto color: colors) {
            for (auto index = 1; index <= 12; index++, i++) {
                auto color_mod = rgb_brightness(color, (float) index * index / (12 * 12 * 2));
                circle.setPixel(index - 1, color_mod);
                circle.show(12 - ((i >> 3) % (12 - 1)));
                sleep_ms(25);
            }
        }
    }

    return 0;
}
