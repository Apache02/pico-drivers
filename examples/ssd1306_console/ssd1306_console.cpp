#include <stdio.h>
#include <string.h>
#include "pico/stdlib.h"
#include "pico/time.h"
#include "pico/binary_info.h"
#include "drivers/led/led.h"
#include "drivers/ssd1306/Ssd1306.h"
#include "drivers/console/Console.h"

// board config
#define LED_PIN PICO_DEFAULT_LED_PIN

#if defined(DISPLAY_SPI)
#define DISPLAY_SPI_SDA 11
#define DISPLAY_SPI_SCL 10
#define DISPLAY_SPI_CS 9
#define DISPLAY_SPI_DC 12
#define DISPLAY_SPI_RES 13
#define DISPLAY_SPI_WIDTH 128
#define DISPLAY_SPI_HEIGHT 64

DeviceIO_SPI io1(DISPLAY_SPI_SDA, DISPLAY_SPI_SCL, DISPLAY_SPI_CS, DISPLAY_SPI_DC, DISPLAY_SPI_RES);
Ssd1306 display_spi(&io1, DISPLAY_SPI_WIDTH, DISPLAY_SPI_HEIGHT);
#endif

#if defined(DISPLAY_I2C)
#define DISPLAY_I2C_SDA 14
#define DISPLAY_I2C_SCL 15
#define DISPLAY_I2C_ADDRESS SSD1306_DEFAULT_I2C_ADDRESS
#define DISPLAY_I2C_WIDTH 128
#define DISPLAY_I2C_HEIGHT 32

DeviceIO_I2C io2(DISPLAY_I2C_SDA, DISPLAY_I2C_SCL, DISPLAY_I2C_ADDRESS);
Ssd1306 display_i2c(&io2, DISPLAY_I2C_WIDTH, DISPLAY_I2C_HEIGHT);
#endif


LED led(LED_PIN);

const char text[] = "What is Lorem Ipsum?\n"
                    "Lorem Ipsum is simply dummy text of the printing and typesetting industry. "
                    "Lorem Ipsum has been the industry's standard dummy text ever since the 1500s, "
                    "when an unknown printer took a galley of type and scrambled it to make a "
                    "type specimen book.";


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
    srand(time_us_32());

#if defined(DISPLAY_SPI)
    display_spi.init();
    Console console1(display_spi.buffer, display_spi.width, display_spi.height);
    console1.clear();
    display_spi.update();
#endif
#if defined(DISPLAY_I2C)
    display_i2c.init();
    Console console2(display_i2c.buffer, display_i2c.width, display_i2c.height);
    console2.clear();
    display_i2c.update();
#endif

    struct DisplayAndConsole {
        Ssd1306 *display;
        Console *console;
    } displays[] = {
#if defined(DISPLAY_SPI)
            {&display_spi, &console1},
#endif
#if defined(DISPLAY_I2C)
            {&display_i2c, &console2},
#endif
    };

    for (;;) {
        for (auto[display, console]: displays) {
            console->clear();
            for (auto ptr = text; *ptr != '\0'; ptr++) {
                char tmp[2] = {*ptr, '\0'};
                console->print(tmp);
                display->update();
                sleep_ms(25);
            }
        }
        sleep_ms(1000);

        for (auto[display, console]: displays) {
            char tmp[16];
            console->print("\n");
            for (auto i = 0; i <= 100; i++) {
                console->clearLine();
                console->printf("%d%% ", i);
                for (auto j = 0; j < ((i >> 1) % 4); j++) {
                    console->print(".");
                }
                if (i == 100) {
                    console->print(" DONE");
                }
                display->update();
                sleep_ms(50);
            }
            console->print("\n");
        }
        sleep_ms(1000);
    }

    return 0;
}
