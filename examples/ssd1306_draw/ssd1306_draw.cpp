#include <stdio.h>
#include <string.h>
#include "pico/stdlib.h"
#include "pico/time.h"
#include "pico/binary_info.h"
#include "drivers/led/led.h"
#include "drivers/ssd1306/Ssd1306.h"
#include "drivers/gui/Gui.h"
#include "drivers/fonts/fonts.h"

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


void startup_test() {
    printf("%s START\n", __FUNCTION__);
    led.on();
    sleep_ms(2000);
    led.off();
    sleep_ms(10);
    printf("%s DONE\n", __FUNCTION__);
}

void test_drawCircles(Ssd1306 *d, Gui *gui) {
    printf("%s START\n", __PRETTY_FUNCTION__);
    gui->fill(0);
    for (auto i = 0; i < 10; i++) {
        Gui::Point center = {int16_t(rand() & (d->width - 1) - 16), int16_t(rand() & (d->height - 1) - 16)};
        int16_t radius = (rand() & 63) - 32;
        if (rand() & 1) {
            gui->drawCircle(center, radius);
        } else {
            gui->fillCircle(center, radius);
        }
        d->update();
        sleep_ms(100);
    }
    printf("%s END\n", __PRETTY_FUNCTION__);
}

void test_drawLines(Ssd1306 *d, Gui *gui) {
    printf("%s START\n", __PRETTY_FUNCTION__);
    gui->fill(0);
    for (auto i = 0; i < 10; i++) {
        Gui::Point p1 = {int16_t(rand() & (d->width - 1) - 16), int16_t(rand() & (d->height - 1) - 16)};
        Gui::Point p2 = {int16_t(rand() & (d->width - 1) - 16), int16_t(rand() & (d->height - 1) - 16)};
        gui->drawLine(p1, p2);
        d->update();
        sleep_ms(100);
    }
    printf("%s END\n", __PRETTY_FUNCTION__);
}

void test_drawRects(Ssd1306 *d, Gui *gui) {
    printf("%s START\n", __PRETTY_FUNCTION__);
    gui->fill(0);
    for (auto i = 0; i < 10; i++) {
        Gui::Rect r = {
                int16_t(rand() & (d->width - 1) - 16), int16_t(rand() & (d->height - 1) - 16),
                int16_t(rand() & (d->width - 1) - 16), int16_t(rand() & (d->height - 1) - 16)
        };
        gui->drawRect(r);
        d->update();
        sleep_ms(100);
    }
    printf("%s END\n", __PRETTY_FUNCTION__);
}

void test_drawGlyphs(Ssd1306 *d, Gui *gui) {
    printf("%s START\n", __PRETTY_FUNCTION__);
    gui->fill(0);
    const auto *font = &font_5x8;
    gui->setFont(font);
    Gui::Point p = {0, 0};
    int16_t space = 1;
    for (wchar_t c = font->first; c <= font->last; c++) {
        gui->drawGlyph(c, p);
        d->update();
        sleep_ms(1);

        p.x += font->width + space;
        if (p.x + font->width > d->width) {
            p.y += font->height + space;
            p.x = 0;

            if (p.y + font->height > d->height) {
                sleep_ms(1000);
                p.x = 0;
                p.y = 0;
                gui->fill(0);
            }
        }
    }
    printf("%s END\n", __PRETTY_FUNCTION__);
}

void test_drawText(Ssd1306 *d, Gui *gui) {
    printf("%s START\n", __PRETTY_FUNCTION__);
    gui->fill(0);
    const auto *font_2 = &font_5x8;
    gui->setFont(font_2);
    const struct {
        const char *text;
        uint align;
    } examples[] = {
            {"[ 0 ]",           0},

            {"LEFT",            Gui::Align::LEFT},
            {"CENTER",          Gui::Align::CENTER},
            {"RIGHT",           Gui::Align::RIGHT},
            {"TOP",             Gui::Align::TOP},
            {"MIDDLE",          Gui::Align::MIDDLE},
            {"BOTTOM",          Gui::Align::BOTTOM},

            {"LEFT | TOP",      Gui::Align::LEFT | Gui::Align::TOP},
            {"CENTER | TOP",    Gui::Align::CENTER | Gui::Align::TOP},
            {"RIGHT | TOP",     Gui::Align::RIGHT | Gui::Align::TOP},
            {"LEFT | MIDDLE",   Gui::Align::LEFT | Gui::Align::MIDDLE},
            {"CENTER | MIDDLE", Gui::Align::CENTER | Gui::Align::MIDDLE},
            {"RIGHT | MIDDLE",  Gui::Align::RIGHT | Gui::Align::MIDDLE},
            {"LEFT | BOTTOM",   Gui::Align::LEFT | Gui::Align::BOTTOM},
            {"CENTER | BOTTOM", Gui::Align::CENTER | Gui::Align::BOTTOM},
            {"RIGHT | BOTTOM",  Gui::Align::RIGHT | Gui::Align::BOTTOM},
    };
    for (auto example: examples) {
        gui->fill(0);
        gui->drawRect(0, 0, d->width - 1, d->height - 1);
        gui->drawText(
                example.text,
                1,
                {1, 1, int16_t(d->width - 2), int16_t(d->height - 2)},
                Gui::Align(example.align)
        );
        d->update();
        sleep_ms(1000);
    }
    printf("%s END\n", __PRETTY_FUNCTION__);
}

int main() {
    bi_decl(bi_program_description("This is a lib-drivers-example binary."));
    bi_decl(bi_1pin_with_name(LED_PIN, "On-board LED"));

    stdio_init_all();

    startup_test();
    srand(time_us_32());

#if defined(DISPLAY_SPI)
    display_spi.init();
    Gui gui1(display_spi.buffer, display_spi.width, display_spi.height);
    gui1.fill(0);
    display_spi.update();
#endif
#if defined(DISPLAY_I2C)
    display_i2c.init();
    Gui gui2(display_i2c.buffer, display_i2c.width, display_i2c.height);
    gui2.fill(0);
    display_i2c.update();
#endif

    struct _DISPLAYS {
        Ssd1306 *display;
        Gui *gui;
    } displays[] = {
#if defined(DISPLAY_SPI)
            {&display_spi, &gui1},
#endif
#if defined(DISPLAY_I2C)
            {&display_i2c, &gui2},
#endif
    };

    for (;;) {
#if defined(DRAW_CIRCLES)
        for (auto display: displays) {
            test_drawCircles(display.display, display.gui);
        }
        sleep_ms(1000);
#endif

#if defined(DRAW_LINES)
        for (auto display: displays) {
            test_drawLines(display.display, display.gui);
        }
        sleep_ms(1000);
#endif

#if defined(DRAW_RECTS)
        for (auto display: displays) {
            test_drawRects(display.display, display.gui);
        }
        sleep_ms(1000);
#endif

#if defined(DRAW_GLYPHS)
        for (auto display: displays) {
            test_drawGlyphs(display.display, display.gui);
        }
        sleep_ms(1000);
#endif

#if defined(DRAW_TEXT)
        for (auto display: displays) {
            test_drawText(display.display, display.gui);
        }
        sleep_ms(1000);
#endif
    }

    return 0;
}