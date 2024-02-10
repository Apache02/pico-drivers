#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "pico/stdlib.h"
#include "pico/time.h"
#include "pico/binary_info.h"
#include "drivers/led/led.h"
#include "drivers/ssd1680/Ssd1680.h"
#include "drivers/gui/Gui.h"
#include "drivers/fonts/fonts.h"

// board config
#define LED_PIN PICO_DEFAULT_LED_PIN
#define USR_KEY_PIN 24

#define DISPLAY_SPI_SDA 19
#define DISPLAY_SPI_SCL 18
#define DISPLAY_SPI_CS 17
#define DISPLAY_SPI_DC 16
#define DISPLAY_SPI_RES 20
#define DISPLAY_SPI_BUSY 21
#define DISPLAY_SPI_WIDTH 250
#define DISPLAY_SPI_HEIGHT 122

LED led(LED_PIN);
Ssd1680_IO io(DISPLAY_SPI_SDA, DISPLAY_SPI_SCL, DISPLAY_SPI_CS, DISPLAY_SPI_DC, DISPLAY_SPI_RES, DISPLAY_SPI_BUSY);
Ssd1680 display(&io, DISPLAY_SPI_WIDTH, DISPLAY_SPI_HEIGHT);


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
    gpio_init(USR_KEY_PIN);
    gpio_set_dir(USR_KEY_PIN, GPIO_IN);
    gpio_pull_up(USR_KEY_PIN);

    startup_test();
    srand(time_us_32());

    printf("call display.init()\n");
    display.init();
    Gui gui_white(display.buffer_mono, display.width, display.height);
    Gui gui_red(display.buffer_color, display.width, display.height);

    gui_white.setFont(&font_5x8);
    gui_red.setFont(&font_5x8);

    gui_white.fill(0);
    char tmp[32];
    sprintf(tmp, "%dx%d", display.width, display.height);
    gui_white.drawText(tmp, { 0, 0, int16_t (display.width - 1), int16_t (display.height -1)}, 0, Gui::Align(Gui::CENTER | Gui::MIDDLE));
    gui_white.invert();
    gui_red.fill(0);
    display.update();
    led.on();
    display.io->wait_busy();
    led.off();
    sleep_ms(1000);

    for (;;) {
        gui_white.fill(0);
        gui_red.fill(0);

        int16_t x = 0;
        int16_t y = 0;
        for (auto i = 0; x < display.width && y < display.height; i++) {
            int16_t w = 8 * (i + 1);
            int16_t h = w;
            Gui::Rect rect = {x, y, int16_t (x + w - 1), int16_t (y + h - 1)};
            gui_white.fillRect(rect);

            char tmp[32];
            sprintf(tmp, "%dx%d", w, h);
            gui_red.drawText(tmp, rect, 0, Gui::Align(Gui::CENTER | Gui::MIDDLE));
            printf("fillRect [(%d; %d), (%d; %d)]\n", rect.x1, rect.y1, rect.x2, rect.y2);

            x += w;
            y += h;
        }

        char tmp[32];
        sprintf(tmp, "%dx%d", display.width, display.height);
        gui_white.drawText(tmp, { 0, 0, int16_t (display.width - 1), int16_t (display.height -1)}, 0, Gui::Align(Gui::CENTER | Gui::MIDDLE));

        display.update();
        led.on();
        display.io->wait_busy();
        led.off();

        while (gpio_get(USR_KEY_PIN)) {
            sleep_ms(10);
        }

        led.on();
        sleep_ms(10);
        led.off();
    }

    sleep_ms(3600 * 1000);

    return 0;
}
