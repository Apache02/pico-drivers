#include <stdio.h>
#include <string.h>
#include "pico/stdlib.h"
#include "pico/time.h"
#include "pico/binary_info.h"
#include "drivers/led.h"
#include "drivers/Pwm.h"
#include "drivers/lcd/display.h"

// board config
#define LED_PIN PICO_DEFAULT_LED_PIN

#define LCD_1602_D0_PIN     5
#define LCD_1602_D1_PIN     6
#define LCD_1602_D2_PIN     7
#define LCD_1602_D3_PIN     8
#define LCD_1602_D4_PIN     9
#define LCD_1602_D5_PIN     10
#define LCD_1602_D6_PIN     11
#define LCD_1602_D7_PIN     12

#define LCD_1602_RS_PIN         19
#define LCD_1602_ENABLE_PIN     20
#define LCD_1602_VO_PIN         18

// global variables
LED led(LED_PIN);
Pwm lcd_contrast(LCD_1602_VO_PIN);
LCD::Direct lcd_io(LCD_1602_RS_PIN, LCD_1602_ENABLE_PIN, LCD_1602_D0_PIN);
LCD::Display lcd(&lcd_io, 2, 16);


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

    lcd_contrast.init(UINT16_MAX, 1000);
    lcd_contrast.set_level(0);
    lcd_contrast.on();

    lcd.configure();
    lcd.on(true, true);

    static const char *const message[] =
            {
                    "RP2040 by", "Raspberry Pi",
                    "A brand new", "microcontroller",
                    "Twin core M0", "Full C SDK",
                    "More power in", "your product",
                    "More beans", "than Heinz!"
            };

    while (1) {
        for (int m = 0; m < sizeof(message) / sizeof(message[0]); m += 2) {
            lcd.clear();
            for (int line = 0; line < 2; line++) {
                lcd.position(line, (16 / 2) - strlen(message[m + line]) / 2);
                const char *s = message[m + line];
                printf("write \"%s\"\n", s);
                while (*s != '\0') {
                    lcd.putChar(*s);
                    if (*s != ' ') {
                        sleep_ms(100);
                    }
                    s++;
                }
            }

            sleep_ms(2000);
        }

        for (int i=0; i<256;) {
            lcd.clear();
            for (int line=0; line < 2; line++) {
                lcd.position(line, 0);
                for (int j=0; j<16; j++, i++) {
                    lcd.putChar(i);
                    sleep_ms(100);
                }
            }

            sleep_ms(2000);
        }
    }

    sleep_us(1000'000'000);

    return 0;
}
