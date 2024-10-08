#include <stdio.h>
#include <string.h>
#include "pico/stdlib.h"
#include "pico/time.h"
#include "pico/rand.h"
#include "pico/binary_info.h"
#include "hardware/clocks.h"
#include "drivers/led.h"
#include "drivers/ssd1306/display.h"
#include "drivers/Gui.h"
#include "drivers/fonts.h"

// board config
#define LED_PIN PICO_DEFAULT_LED_PIN
#if defined(DISPLAY_SPI)
#define DISPLAY_SPI_SDA 19
#define DISPLAY_SPI_SCL 18
#define DISPLAY_SPI_CS 17
#define DISPLAY_SPI_DC 16
#define DISPLAY_SPI_RES 22
#define DISPLAY_WIDTH 128
#define DISPLAY_HEIGHT 64

IO::SPI spi(DISPLAY_SPI_SDA, -1, DISPLAY_SPI_SCL);
ssd1306::SPI display_io(spi, DISPLAY_SPI_CS, DISPLAY_SPI_DC, DISPLAY_SPI_RES);
const char connection_type[] = "SPI";

#elif defined(DISPLAY_I2C) || defined(DISPLAY_PIO_I2C)

#define DISPLAY_I2C_SDA 20
#define DISPLAY_I2C_SCL 21

#if defined(DISPLAY_I2C) && !defined(DISPLAY_PIO_I2C)
IO::I2C iic(DISPLAY_I2C_SDA, DISPLAY_I2C_SCL);
const char connection_type[] = "I2C";
#elif defined(DISPLAY_PIO_I2C) && !defined(DISPLAY_I2C)

#include "drivers/PioI2C.h"

IO::PioI2C iic(pio0, DISPLAY_I2C_SDA, DISPLAY_I2C_SCL);

const char connection_type[] = "PIO I2C";
#endif

#define DISPLAY_I2C_ADDRESS SSD1306_DEFAULT_I2C_ADDRESS
#define DISPLAY_WIDTH 128
#define DISPLAY_HEIGHT 32

ssd1306::I2C display_io(iic, DISPLAY_I2C_ADDRESS);


#else
#error "required definition of DISPLAY_SPI or DISPLAY_I2C"
#endif

// global variables
LED led(LED_PIN);
ssd1306::Display display(display_io, DISPLAY_WIDTH, DISPLAY_HEIGHT);


unsigned int get_fps() {
    static uint64_t prevUs = 0;
    uint64_t currUs = time_us_64();
    unsigned int fps = prevUs ? 1000000 / (currUs - prevUs) : 0;
    prevUs = currUs;
    return fps;
}

void hardware_init() {
    set_sys_clock_khz(240000, false);

#if defined(DISPLAY_SPI)
    spi.init(IO::SPI_BAUDRATE_MAX);
#elif defined(DISPLAY_I2C)
    iic.init(3.2 * 1'000'000);
#elif defined(DISPLAY_PIO_I2C)
    iic.init(91 * 1'000'000);
#endif

    display.init();
}

int main() {
    bi_decl(bi_program_description("This is a pico-drivers-example binary."));
    bi_decl(bi_1pin_with_name(LED_PIN, "On-board LED"));

    stdio_init_all();
    hardware_init();
    srand(time_us_32());

    Gui gui(display.buffer, display.width, display.height);
    gui.fill(0);
    display.update();

    gui.setFont(&font_12x16);

    gui.drawText(
            connection_type,
            {0, 0, static_cast<int16_t>(display.width), static_cast<int16_t>(display.height)},
            0, 0,
            Gui::MIDDLE | Gui::CENTER
    );
    display.update();

    led.on();
    sleep_ms(2000);
    led.off();

    printf("clk_sys = %lu Hz\n", clock_get_hz(clk_sys));

    gui.setFont(&font_5x8);

    uint8_t ballR = 3;
    Gui::Point ball = {int16_t(get_rand_32() & (display.width - 1)), int16_t(get_rand_32() & (display.height - 1))};
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
                Gui::MIDDLE | Gui::CENTER
        );

        gui.fillCircle(ball, ballR);
        display.update();

        led.set(hit);
        sleep_ms(10);
    }

    return 0;
}
