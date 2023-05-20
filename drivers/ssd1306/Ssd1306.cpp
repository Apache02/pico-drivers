#include "Ssd1306.h"

enum cmd {
    // Fundamental commands

    SET_CONTRAST = 0x81,         // Double byte command to select 1 out of
                                 // 256 contrast steps.
    SET_ENTIRE_DISP_ON = 0xA4,   // Bit0 = 0: Output follows RAM content.
                                 // Bit0 = 1: Output ignores RAM content,
                                 //           all pixels are turned on.
    SET_NORMAL_INVERTED = 0xA6,  // Bit0 = 0: Normal display.
                                 // Bit0 = 1: Inverted display.
    SET_DISP_ON_OFF = 0xAE,      // Bit0 = 0: Display off, sleep mode.
                                 // Bit0 = 1: Display on, normal mode.

    // Addressing setting Commands

    SET_ADDRESSING_MODE = 0x20,  // Double byte command to set memory
                                 // addressing mode.
    SET_COLUMN_ADDRESS = 0x21,   // Tripple byte command to setup column start
                                 // and end address.
    SET_PAGE_ADDRESS = 0x22,     // Tripple byte command to setup page start and
                                 // end address.

    // Hardware configuration (panel resolution and layout related) commands

    SET_DISP_START_LINE = 0x40,  // Set display RAM display start line
                                 // register. Valid values are 0 to 63.
    SET_SEGMENT_REMAP = 0xA0,    // Bit 0 = 0: Map col addr 0 to SEG0.
                                 // Bit 0 = 1: Map col addr 127 to SEG0.
    SET_MUX_RATIO = 0xA8,        // Double byte command to configure display
                                 // height. Valid height values are 15 to 63.
    SET_COM_OUTPUT_DIR = 0xC0,   // Bit 3 = 0: Scan from 0 to N-1.
                                 // Bit 3 = 1: Scan from N-1 to 0. (N=height)
    SET_DISP_OFFSET = 0xD3,      // Double byte command to configure vertical
                                 // display shift. Valid values are 0 to 63.
    SET_COM_PINS_CONFIG = 0xda,  // Double byte command to set COM pins
                                 // hardware configuration.

    // Timing and driving scheme setting commands

    SET_DCLK_FOSC = 0xd5,        // Double byte command to set display clock
                                 // divide ratio and oscillator frequency.
    SET_PRECHARGE_PERIOD = 0xd9, // Double byte command to set pre-charge
                                 // period.
    SET_VCOM_DESEL_LEVEL = 0xdb, // Double byte command to set VCOMH deselect
                                 // level.

    // Charge pump command

    SET_CHARGE_PUMP = 0x8d,      // Double byte command to enable/disable
                                 // charge pump.
                                 // Byte2 = 0x10: Disable charge pump.
                                 // Byte2 = 0x14: Enable charge pump.
};

void Ssd1306::init() {
    const uint8_t x1 = 0, x2 = width - 1;
    const uint8_t p1 = 0, p2 = (height / 8) - 1;
    const uint8_t y1 = 0, y2 = height - 1;
    const uint8_t pins_config = height == 32 ? 0x02 : 0x12;

    const uint8_t commands[] = {
            SET_DISP_ON_OFF | 0x00,             // Display off.
            SET_DCLK_FOSC, 0x80,                // Set clock divide ratio and oscillator frequency.
            SET_MUX_RATIO, y2,                  // Set display height.
            SET_DISP_OFFSET, 0x00,              // Set vertical display shift to 0.
            SET_DISP_START_LINE,                // Set display RAM display start line register to 0.
            SET_CHARGE_PUMP, 0x14,              // Enable charge pump.
            SET_SEGMENT_REMAP | 0x01,           // Map col addr 127 to SEG0.
            SET_COM_OUTPUT_DIR | 0x08,          // Scan from N-1 to 0. (N=height)
            SET_COM_PINS_CONFIG, pins_config,   // Set COM pins hardware configuration.
            SET_CONTRAST, 0xcf,                 // Set contrast to 0xcf
            SET_PRECHARGE_PERIOD, 0xf1,         // Set pre-charge to 0xf1
            SET_VCOM_DESEL_LEVEL, 0x40,         // Set VCOMH deselect to 0x40
            SET_ENTIRE_DISP_ON,                 // Output follows RAM content.
            SET_NORMAL_INVERTED | 0x00,         // Normal display.
            SET_ADDRESSING_MODE, 0x00,          // Set addressing mode to horizontal mode.
            SET_COLUMN_ADDRESS, x1, x2,         // Set column start and end address.
            SET_PAGE_ADDRESS, p1, p2,           // Set page start and end address.
            SET_DISP_ON_OFF | 0x01,             // Display on.
    };
    io->commands(commands, sizeof(commands));

    bufferSize = (width * height / 8);
    buffer = new uint8_t[bufferSize];
}

Ssd1306::~Ssd1306() {
    if (buffer) {
        delete buffer;
        buffer = nullptr;
    }
}

Ssd1306::Ssd1306(DeviceIO *io, uint8_t width, uint8_t height) : io(io), width(width), height(height) {}

void Ssd1306::update() {
    if (!buffer) {
        return;
    }

    io->write(buffer, bufferSize);
}
