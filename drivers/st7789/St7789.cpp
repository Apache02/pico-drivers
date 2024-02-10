#include "St7789.h"

#define FILL_BUFFER_SIZE 32

enum reg {
    NOP = 0x00,
    SWRESET = 0x01,
    RDDID = 0x04,
    RDDST = 0x09,
    RDDPM = 0x0A,
    SLPIN = 0x10,
    SLPOUT = 0x11,
    PTLON = 0x12,
    NORON = 0x13,
    INVOFF = 0x20,
    INVON = 0x21,
    GAMSET = 0x26,
    DISPOFF = 0x28,
    DISPON = 0x29,
    CASET = 0x2A,
    RASET = 0x2B,
    RAMWR = 0x2C,
    RAMRD = 0x2E,
    PTLAR = 0x30,
    VSCRDEF = 0x33,
    TEOFF = 0x34,
    TEON = 0x35,
    MADCTL = 0x36,
    VSCRSADD = 0x37,
    IDMOFF = 0x38,
    IDMON = 0x39,
    COLMOD = 0x3A,
    RAMWRC = 0x3C,
    RAMRDC = 0x3E,
    WRDISBV = 0x51,
    RDDISBV = 0x52,
    WRCTRLD = 0x53,
    RDCTRLD = 0x54,
    PORCTRL = 0xB2,
    GCTRL = 0xB7,
    VCOMS = 0xBB,
    LCMCTRL = 0xC0,
    VDVVRHEN = 0xC2,
    VRHS = 0xC3,
    VDVS = 0xC4,
    FRCTRL2 = 0xC6,
    PWMFRSEL = 0xCC,
    PWCTRL1 = 0xD0,
    GMCTRP1 = 0xE0,
    GMCTRN1 = 0xE1,
};

void St7789::internal_init() {

    io->command(reg::SWRESET);
    sleep_ms(100);

    io->command(reg::TEON);  // enable frame sync signal if used
    io->command(reg::COLMOD, (const uint8_t *) "\x55", 1);  // 16 bits per pixel
    if (width == 320 && height == 240) {
        io->command(reg::MADCTL, (const uint8_t *) "\x70", 1);
    } else {
        io->command(reg::MADCTL, (const uint8_t *) "\x00", 1);
    }

    io->command(reg::PORCTRL, (const uint8_t *) "\x0c\x0c\x00\x33\x33", 5);
    io->command(reg::LCMCTRL, (const uint8_t *) "\x2c", 1);
    io->command(reg::VDVVRHEN, (const uint8_t *) "\x01", 1);
    io->command(reg::VRHS, (const uint8_t *) "\x12", 1);
    io->command(reg::VDVS, (const uint8_t *) "\x20", 1);
    io->command(reg::PWCTRL1, (const uint8_t *) "\xa4\xa1", 2);
    io->command(reg::FRCTRL2, (const uint8_t *) "\x0f", 1);

    io->command(reg::NORON);
    fill(0);

    io->command(reg::INVON);   // set inversion mode
    io->command(reg::SLPOUT);  // leave sleep mode
    io->command(reg::DISPON);  // turn display on

    sleep_ms(100);
}

St7789::~St7789() {
}

void St7789::working_area(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2) {
    uint16_t CASET_data[] = {x1, x2};
    uint16_t RASET_data[] = {y1, y2};
    io->command(reg::CASET, CASET_data, 2); // X
    io->command(reg::RASET, RASET_data, 2); // Y
}

void St7789::fill(uint16_t color) {
    const size_t length = FILL_BUFFER_SIZE;
    uint16_t buffer[length];
    uint16_t *ptr = buffer;
    for (size_t n = length; n > 0; n--) {
        *ptr++ = color;
    }

    working_area(0, 0, width - 1, height - 1);
    io->command(reg::RAMWR);

    for (size_t n = width * height; n > 0;) {
        if (n >= length) {
            io->write(buffer, length);
            n -= length;
        } else {
            io->write(buffer, n);
            n = 0;
        }
    }
}

void St7789::pixel(uint16_t x, uint16_t y, uint16_t color) {
    working_area(x, y, x, y);
    io->command(reg::RAMWR, &color, 1);
}

void St7789::draw(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, const uint16_t *buffer, size_t length) {
    working_area(x1, y1, x2, y2);
    io->command(reg::RAMWR, buffer, length);
}
