#include <stdio.h>
#include "pico/stdlib.h"
#include "pico/time.h"
#include "pico/binary_info.h"
#include "drivers/led.h"

// board config
#define LED_PIN PICO_DEFAULT_LED_PIN

#define INTERVAL        150

// global variables
LED led(LED_PIN);


void startup_test() {
    printf("%s START\n", __FUNCTION__);
    led.on();
    sleep_ms(2000);
    led.off();
    sleep_ms(10);
    printf("%s DONE\n", __FUNCTION__);
}

void dot() {
    led.on();
    sleep_ms(INTERVAL);
    led.off();
    sleep_ms(INTERVAL);
}

void dash() {
    led.on();
    sleep_ms(INTERVAL * 3);
    led.off();
    sleep_ms(INTERVAL);
}

int main() {
    bi_decl(bi_program_description("This is a pico-drivers-example binary."));
    bi_decl(bi_1pin_with_name(LED_PIN, "On-board LED"));

    stdio_init_all();

    startup_test();

    while (true) {
        dot();
        dot();
        dot();
        dash();
        dash();
        dash();
        dot();
        dot();
        dot();
        sleep_ms(INTERVAL * 2);
    }

    return 0;
}
