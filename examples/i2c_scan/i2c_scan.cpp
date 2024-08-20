#include <stdio.h>
#include <string.h>
#include "pico/stdlib.h"
#include "pico/binary_info.h"
#include "drivers/led.h"
#include "drivers/I2C.h"

// board config
#define LED_PIN PICO_DEFAULT_LED_PIN
#define I2C_SDA_PIN 20
#define I2C_SCL_PIN 21


bool reserved_addr(uint8_t addr) {
    return (addr & 0x78) == 0 || (addr & 0x78) == 0x78;
}

int main() {
    bi_decl(bi_program_description("This is a pico-drivers-example binary."));
    bi_decl(bi_1pin_with_name(LED_PIN, "On-board LED"));

    auto led = LED(LED_PIN);

    stdio_init_all();
    led.on();
    sleep_ms(2000);
    led.off();

    auto io = IO::I2C(I2C_SDA_PIN, I2C_SCL_PIN);
    io.init(100'000);

    printf("\nPIO I2C Bus Scan\n");
    printf("   0  1  2  3  4  5  6  7  8  9  A  B  C  D  E  F\n");

    for (int addr = 0; addr < (1 << 7); ++addr) {
        if (addr % 16 == 0) {
            printf("%02x ", addr);
        }

        // Perform a 1-byte dummy read from the probe address. If a slave
        // acknowledges this address, the function returns the number of bytes
        // transferred. If the address byte is ignored, the function returns
        // -1.

        int result;
        uint8_t rx;
        if (reserved_addr(addr)) {
            result = PICO_ERROR_GENERIC;
        } else {
            result = io.read(addr, &rx, 1, true);
        }

        printf(result < 0 ? "." : "@");
        printf(addr % 16 == 15 ? "\n" : "  ");
    }

    printf("Done.\n");

    for (;;) {
        led.on();
        sleep_ms(500);
        led.off();
        sleep_ms(500);
    }

    return 0;
}
