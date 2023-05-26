#include "Ssd1680.h"
#include <initializer_list>
#include <stdio.h>

#define UINT16_TO_LE_BYTES(n) uint8_t (n & 0xFF), uint8_t ((n >> 8) & 0xFF)

enum reg {
    DOCTL = 0x01,           // Driver Output Control
    GDVCTL = 0x03,          // Gate Driving voltage Control
    DEEPSLEEP = 0x10,       // Deep Sleep mode
    DEMOD = 0x11,           // Data Entry mode setting
                            // Address automatic increment / decrement setting
                            // 0x01 - X increment
                            // 0x02 - Y increment
                            // 0x04 - Y direction

    SWRESET = 0x12,         // Software reset
    TSCTL = 0x18,           // Temperature Sensor Control
    MA = 0x20,              // Master Activation
    DUCTL1 = 0x21,          // Display Update Control 1
    DUCTL2 = 0x22,          // Display Update Control 2
    RAMWR_WHITE = 0x24,     // Write RAM (Black White) | 1 = WHITE, 0 = BLACK
    RAMWR_COLOR = 0x26,     // Write RAM (RED) | 1 = RED
    RAMRD = 0x27,           // Read RAM
    BWFCTL = 0x3C,          // Border Waveform Control
    SET_X = 0x44,           // Set RAM X - address | Start / End position
    SET_Y = 0x45,           // Set Ram Y - address | Start / End position
    SET_X_ADDR = 0x4E,      // Set RAM X address
    SET_Y_ADDR = 0x4F,      // Set RAM Y address
    NOP = 0x7F,
};

struct command_t {
    uint8_t reg;
    std::initializer_list<uint8_t> data;
};

void Ssd1680::init() {
    io->wait_busy();
    io->command(reg::SWRESET);
    io->wait_busy();

    const uint16_t height2 = (height + 0b111) >> 3 << 3;
    const uint16_t y1 = 0, y2 = width - 1;
    const uint8_t x1 = 0, x2 = (height2 >> 3) - 1;

    const command_t commands[] = {
            {DOCTL,      {uint8_t(y2 & 0xFF), uint8_t((y2 >> 8) & 0xFF), 0x00}},
            {DEMOD,      {0b0100}},
            {SET_X,      {x2, x1}},
            {SET_Y,      {UINT16_TO_LE_BYTES(y2), UINT16_TO_LE_BYTES(y1)}},
            {BWFCTL,     {0x05}},
            {DUCTL1,     {0x00, 0x80}},
            {TSCTL,      {0x80}},
            // Set RAM address
            {SET_X_ADDR, {x2}},
            {SET_Y_ADDR, {UINT16_TO_LE_BYTES(y2)}},
    };

    for (auto item: commands) {
        io->command(item.reg, item.data.begin(), item.data.size());
    }

    io->wait_busy();

    bufferSize = (width * (height2 >> 3));
    buffer_mono = new uint8_t[bufferSize];
    buffer_color = new uint8_t[bufferSize];
}

Ssd1680::~Ssd1680() {
    if (buffer_mono) {
        delete buffer_mono;
        buffer_mono = nullptr;
    }
    if (buffer_color) {
        delete buffer_color;
        buffer_color = nullptr;
    }
}

void Ssd1680::update() {
    if (!buffer_mono && !buffer_color) {
        return;
    }

    io->wait_busy();

    const uint16_t pagesCount = (height + 0b111) >> 3;
    for (uint8_t i = 0; i <= pagesCount; i++) {
        io->command(SET_X_ADDR, &i, 1);
        io->command(RAMWR_WHITE, &buffer_mono[i * width], width);
        io->command(SET_X_ADDR, &i, 1);
        io->command(RAMWR_COLOR, &buffer_color[i * width], width);
    }

    io->wait_busy();
    const command_t command = {DUCTL2, {0xF7}};
    io->command(command.reg, command.data.begin(), command.data.size());
    io->command(MA);
}
