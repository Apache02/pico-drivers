#include <stdio.h>
#include <math.h>
#include <string.h>
#include "pico/stdlib.h"
#include "pico/time.h"
#include "hardware/interp.h"
#include "pico/binary_info.h"
#include "drivers/led.h"
#include "drivers/SPI.h"
#include "drivers/st7789/io.h"
#include "drivers/st7789/display.h"

#include "raspberry_256x256_rgb565.h"


// board config
#define LED_PIN PICO_DEFAULT_LED_PIN

#define DISPLAY_SPI_SDA          19
#define DISPLAY_SPI_SCL          18
#define DISPLAY_SPI_CS           17
#define DISPLAY_SPI_DC           16
#define DISPLAY_SPI_RES          22
#define DISPLAY_SPI_WIDTH        240
#define DISPLAY_SPI_HEIGHT       320

#define BUFFER_ROWS_COUNT        32

// global variables
LED led(LED_PIN);
IO::SPI spi(DISPLAY_SPI_SDA, -1, DISPLAY_SPI_SCL);
//st7789::IO_Spi display_io(spi, DISPLAY_SPI_CS, DISPLAY_SPI_DC, DISPLAY_SPI_RES);
st7789::IO_Spi_Dma display_io(spi, DISPLAY_SPI_CS, DISPLAY_SPI_DC, DISPLAY_SPI_RES);
st7789::Display display(&display_io, DISPLAY_SPI_WIDTH, DISPLAY_SPI_HEIGHT);


typedef struct {
    uint16_t pixels[BUFFER_ROWS_COUNT][DISPLAY_SPI_WIDTH];
} pixels_buffer_t;

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
    unsigned int fps;
    if (!prevUs) {
        fps = 0;
    } else {
        fps = 1'000'000 / (currUs - prevUs);
    }
    prevUs = currUs;
    return fps;
}

int main() {
    bi_decl(bi_program_description("This is a pico-drivers-example binary."));
    bi_decl(bi_1pin_with_name(LED_PIN, "On-board LED"));

    stdio_init_all();
    spi.init(IO::SPI_BAUDRATE_MAX);
    display_io.init();
    display.init();

    startup_test();

    printf("start %s\n", __FUNCTION__);
    printf("image size %d\n", sizeof(raspberry_256x256));
    printf("buffer size: %d / %d rows\n", sizeof(pixels_buffer_t), BUFFER_ROWS_COUNT);

    // partial display buffers
    pixels_buffer_t *buffers[2] = {
            new pixels_buffer_t,
            new pixels_buffer_t,
    };

#ifdef COPY_IMAGE_TO_RAM
    uint16_t *image_ram = new uint16_t[sizeof(raspberry_256x256) / sizeof(uint16_t)];
    memcpy(image_ram, raspberry_256x256, sizeof(raspberry_256x256));
    const uint16_t *image = image_ram;
#else
    const uint16_t * image = (const uint16_t *) &raspberry_256x256[0];
#endif

#define UNIT_LSB 16
#define LOG_IMAGE_SIZE 8
    interp_config lane0_cfg = interp_default_config();
    interp_config_set_shift(&lane0_cfg, UNIT_LSB - 1); // -1 because 2 bytes per pixel
    interp_config_set_mask(&lane0_cfg, 1, 1 + (LOG_IMAGE_SIZE - 1));
    interp_config_set_add_raw(&lane0_cfg, true); // Add full accumulator to base with each POP
    interp_config lane1_cfg = interp_default_config();
    interp_config_set_shift(&lane1_cfg, UNIT_LSB - (1 + LOG_IMAGE_SIZE));
    interp_config_set_mask(&lane1_cfg, 1 + LOG_IMAGE_SIZE, 1 + (2 * LOG_IMAGE_SIZE - 1));
    interp_config_set_add_raw(&lane1_cfg, true);

    interp_set_config(interp0, 0, &lane0_cfg);
    interp_set_config(interp0, 1, &lane1_cfg);
    interp0->base[2] = (uint32_t) image;

    float theta = 0.f;
    float theta_max = 2.f * (float) M_PI;
    float theta_step = theta_max / 360;

    uint64_t fpsShowCd = time_us_64() + 1000000;
    for (;;) {

        theta += theta_step;
        if (theta > theta_max) {
            theta -= theta_max;
        }
        int32_t rotate[4] = {
                static_cast<int32_t>(cosf(theta) * (1 << UNIT_LSB)),
                static_cast<int32_t>(-sinf(theta) * (1 << UNIT_LSB)),
                static_cast<int32_t>(sinf(theta) * (1 << UNIT_LSB)),
                static_cast<int32_t>(cosf(theta) * (1 << UNIT_LSB)),
        };
        interp0->base[0] = rotate[0];
        interp0->base[1] = rotate[2];

        unsigned int y, bi, by;
        for (y = 0, bi = 0; y < display.height; y += by, bi = (bi + 1) & 1) {
            pixels_buffer_t *buffer = buffers[bi];

            for (by = 0; by < BUFFER_ROWS_COUNT; ++by) {
                interp0->accum[0] = rotate[1] * (y + by);
                interp0->accum[1] = rotate[3] * (y + by);
                for (int x = 0; x < display.width; ++x) {
                    buffer->pixels[by][x] = *(uint16_t *) (interp0->pop[2]);
                }
            }

            // send lines to display
            display.draw(0, y, display.width - 1, y + by - 1, &buffer->pixels[0][0], by * display.width);
        }

        auto fps = get_fps();
        if (time_us_64() > fpsShowCd) {
            printf("fps: %d\n", fps);
            fpsShowCd = time_us_64() + 1000000;
        }
    }

    printf("finish %s\n", __FUNCTION__);

    return 0;
}
