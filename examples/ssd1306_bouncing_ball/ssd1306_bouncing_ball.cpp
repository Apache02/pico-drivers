#include <stdio.h>
#include <string.h>
#include "pico/stdlib.h"
#include "pico/time.h"
#include "pico/binary_info.h"
#include "drivers/led.h"
#include "drivers/ssd1306/display.h"
#include "drivers/Gui.h"
#include "drivers/fonts.h"

// board config
#define LED_PIN PICO_DEFAULT_LED_PIN
#if defined(DISPLAY_SPI)
#define DISPLAY_SPI_SDA 15
#define DISPLAY_SPI_SCL 14
#define DISPLAY_SPI_CS 17
#define DISPLAY_SPI_DC 16
#define DISPLAY_SPI_RES 13
#define DISPLAY_WIDTH 128
#define DISPLAY_HEIGHT 64
ssd1306::SPI io(DISPLAY_SPI_SDA, DISPLAY_SPI_SCL, DISPLAY_SPI_CS, DISPLAY_SPI_DC, DISPLAY_SPI_RES);
#elif defined(DISPLAY_I2C)
#define DISPLAY_I2C_SDA 14
#define DISPLAY_I2C_SCL 15
#define DISPLAY_I2C_ADDRESS SSD1306_DEFAULT_I2C_ADDRESS
#define DISPLAY_WIDTH 128
#define DISPLAY_HEIGHT 32
DeviceIO_I2C io(DISPLAY_I2C_SDA, DISPLAY_I2C_SCL, DISPLAY_I2C_ADDRESS);
#else
#error "required definition of DISPLAY_SPI or DISPLAY_I2C"
#endif

// global variables
LED led(LED_PIN);
ssd1306::Display display(&io, DISPLAY_WIDTH, DISPLAY_HEIGHT);


void startup_test() {
    printf("%s START\n", __FUNCTION__);
    led.on();
    sleep_ms(2000);
    led.off();
    sleep_ms(10);
    printf("%s DONE\n", __FUNCTION__);
}

unsigned int get_fps() {
    static uint64_t prevUs = 0;
    uint64_t currUs = time_us_64();
    unsigned int fps = prevUs ? 1000000 / (currUs - prevUs) : 0;
    prevUs = currUs;
    return fps;
}

int main() {
    bi_decl(bi_program_description("This is a pico-drivers-example binary."));
    bi_decl(bi_1pin_with_name(LED_PIN, "On-board LED"));

    stdio_init_all();

    startup_test();
    display.init();

    srand(time_us_32());
    Gui gui(display.buffer, display.width, display.height);

    gui.fill(0);
    gui.setFont(&font_5x8);
    display.update();

    uint8_t ballR = 3;
    Gui::Point ball = {int16_t(rand() & 0x3f), int16_t(rand() & 0x3f)};
    if (ball.x < ballR) ball.x += ballR;
    if (ball.x > display.width - ballR) ball.x -= ballR;
    if (ball.y < ballR) ball.y += ballR;
    if (ball.y > display.height - ballR) ball.y -= ballR;

    int16_t dir_x = 1;
    int16_t dir_y = 1;

    for (;;) {
        bool hit = false;

        gui.fill(0);
        gui.drawRect(0, 0, display.width - 1, display.height - 1);

        ball.x += dir_x;
        ball.y += dir_y;

        if ((ball.x - ballR <= 0) || (ball.x + ballR >= display.width - 1)) {
            hit = true;
            dir_x *= -1;
            ball.x += dir_x;
        }

        if ((ball.y - ballR <= 0) || (ball.y + ballR >= display.height - 1)) {
            hit = true;
            dir_y *= -1;
            ball.y += dir_y;
        }

        int fps = get_fps();
        char fpsBuffer[32];
        sprintf(fpsBuffer, "fps: %d", fps);
        gui.drawText(
                fpsBuffer,
                {1, 1, static_cast<int16_t>(display.width - 2), static_cast<int16_t>(display.height - 2)},
                0,
                0,
                Gui::TOP | Gui::RIGHT
        );

        gui.fillCircle(ball, ballR);
        display.update();

        led.set(hit);
        sleep_ms(10);
    }

    return 0;
}
