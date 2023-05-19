#include <stdio.h>
#include "pico/stdlib.h"
#include "pico/time.h"
#include "pico/binary_info.h"
#include "drivers/led/led.h"
#include "drivers/ssd1306/Ssd1306.h"
#include <string.h>

// board config
#define LED_PIN PICO_DEFAULT_LED_PIN
#define DISPLAY_I2C_SDA 11
#define DISPLAY_I2C_SCL 10
#define DISPLAY_I2C_CS 9
#define DISPLAY_I2C_DC 12
#define DISPLAY_I2C_RES 13
#define DISPLAY_WIDTH 128
#define DISPLAY_HEIGHT 64

// global variables
LED led(LED_PIN);
DeviceIO_SPI io(DISPLAY_I2C_SDA, DISPLAY_I2C_SCL, DISPLAY_I2C_CS, DISPLAY_I2C_DC, DISPLAY_I2C_RES);
Ssd1306 display(&io, DISPLAY_WIDTH, DISPLAY_HEIGHT);


void startup_test() {
    printf("%s START\n", __FUNCTION__);
    led.on();
    sleep_ms(2000);
    led.off();
    sleep_ms(10);
    printf("%s DONE\n", __FUNCTION__);
}

int main() {
    bi_decl(bi_program_description("This is a lib-drivers-example binary."));
    bi_decl(bi_1pin_with_name(LED_PIN, "On-board LED"));

    stdio_init_all();

    startup_test();
    display.init();

    static const uint8_t patterns[] = {
            0xFF,
            0x00,
            0xF0,
            0x0F,
            0xAA,
            0x55,
    };

    memset(display.buffer, 0, display.bufferSize);
    while (true) {
        for (auto pattern : patterns) {
            printf("draw pattern 0x%02X\n", pattern);
            for (auto i=0; i<display.bufferSize; i++) {
                display.buffer[i] = pattern & 0xFF;
                display.update();
            }
            sleep_ms(1000);
        }
    }

    return 0;
}
