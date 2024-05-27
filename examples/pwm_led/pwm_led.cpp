#include <stdio.h>
#include "pico/stdlib.h"
#include "pico/time.h"
#include "pico/binary_info.h"
#include "drivers/pwm_led.h"

// board config
#define LED_PIN                     PICO_DEFAULT_LED_PIN

// global variables
PwmLED led(LED_PIN);


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

    int value = 0;

    for (;;) {
        while (value++ < PwmLED::MAX_VALUE) {
            led.set(value);
            sleep_ms(1);
        }
        sleep_ms(100);

        while (value-- > 0) {
            led.set(value);
            sleep_ms(1);
        }
        sleep_ms(100);
    }

    return 0;
}
