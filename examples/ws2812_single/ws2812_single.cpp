#include <stdio.h>
#include "pico/stdlib.h"
#include "pico/time.h"
#include "pico/binary_info.h"
#include "drivers/led.h"
#include "drivers/Single.h"

// board config
#define LED_PIN             PICO_DEFAULT_LED_PIN
#define WS2812_DATA_PIN     23

// global variables
LED led(LED_PIN);
ws2812::Single led2(WS2812_DATA_PIN, pio0, 0);


void startup_test() {
    printf("%s START\n", __FUNCTION__);
    led.on();
    led2.set(0x0F0000);
    sleep_ms(300);
    led2.set(0x000F00);
    sleep_ms(300);
    led2.set(0x00000F);
    sleep_ms(300);
    led2.set(0);
    sleep_ms(1100);
    led.off();
    sleep_ms(10);
    printf("%s DONE\n", __FUNCTION__);
}

int main() {
    bi_decl(bi_program_description("This is a pico-drivers-example binary."));
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
            led2.set(color);
            sleep_ms(1000);
        }
    }

    return 0;
}
