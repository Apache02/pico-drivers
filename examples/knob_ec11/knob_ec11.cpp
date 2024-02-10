#include <stdio.h>
#include "pico/stdlib.h"
#include "pico/time.h"
#include "pico/binary_info.h"
#include "drivers/led/pwm_led.h"
#include "drivers/knob/ec11.h"

// board config
#define LED_PIN PICO_DEFAULT_LED_PIN
#define KNOB_S1_PIN 4
#define KNOB_S2_PIN 3
#define KNOB_KEY_PIN 2

// global variables
PwmLED led(LED_PIN);
int led_brightness = 0;
EC11 knob(KNOB_S1_PIN, KNOB_S2_PIN, KNOB_KEY_PIN);


void startup_test() {
    printf("%s START\n", __FUNCTION__);
    led.on();
    sleep_ms(2000);
    led.off();
    sleep_ms(10);
    printf("%s DONE\n", __FUNCTION__);
}

int main() {
    bi_decl(bi_program_description("This is a pico-drivers-example binary."));
    bi_decl(bi_1pin_with_name(LED_PIN, "On-board LED"));

    stdio_init_all();

    startup_test();

    static uint64_t last_event_time = 0;
    knob.register_callback([](EC11::EC11_EVENT_T event) {
        const char *name = nullptr;

        static const struct {
            const uint event;
            const char *name;
        } event_name_map[] = {
                {EC11::EMPTY,          ""},
                {EC11::ERROR,          "error     "},
                {EC11::KEY_PRESS,      "press     "},
                {EC11::KEY_LONG_PRESS, "long press"},
                {EC11::KEY_RELEASE,    "release   "},
                {EC11::SIMPLE_CW,      "cw        "},
                {EC11::SIMPLE_CCW,     "ccw       "},
                {EC11::FAST_CW,        "fast cw   "},
                {EC11::FAST_CCW,       "fast ccw  "},
        };

        switch (event) {
            case EC11::SIMPLE_CW:
                led_brightness++;
                break;
            case EC11::SIMPLE_CCW:
                led_brightness--;
                break;
            case EC11::FAST_CW:
                led_brightness += 8;
                break;
            case EC11::FAST_CCW:
                led_brightness -= 8;
                break;
        }

        if (led_brightness > UINT8_MAX) {
            led_brightness = UINT8_MAX;
        } else if (led_brightness < 0) {
            led_brightness = 0;
        }

        for (auto item: event_name_map) {
            if (item.event == event) {
                name = item.name;
                break;
            }
        }

        if (!name) {
            return;
        }

        uint64_t event_time = time_us_64();
        uint64_t event_time_diff = last_event_time ? event_time - last_event_time : 0;

        printf("%f (+%2.06f): %s | %d\n", (float) event_time / 1000000, (float) event_time_diff / 1000000, name,
               led_brightness);
        last_event_time = event_time;
    });

    while (true) {
        auto event = knob.get_event();
        if (event != EC11::EMPTY) {
            if (led_brightness < 127) {
                led.on();
            } else {
                led.off();
            }
            sleep_ms(1);
        }

        led.set((uint8_t) led_brightness);

        sleep_ms(10);
    }

    return 0;
}
