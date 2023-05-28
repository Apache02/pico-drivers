#ifndef LIB_DRIVERS_CONSOLE__H
#define LIB_DRIVERS_CONSOLE__H

#include "pico/stdlib.h"


class Console {
public:
    const uint16_t width;
    const uint16_t height;

private:
    uint8_t *buffer;
    uint16_t offset_x = 0;
    uint16_t offset_line = 0;
    uint8_t lines_count = 0;

    void drawSymbol(const char c, uint16_t x, uint8_t line);

public:
    Console(uint8_t *buffer, uint16_t width, uint16_t height) : buffer(buffer), width(width), height(height) {
        lines_count = height >> 3;
    };

    void clear();

    void print(const char *text);

    int printf(const char *format, ...);

    void printChar(const char c);

    void printLine(const char *text);

    void clearLine();
};

#endif // LIB_DRIVERS_CONSOLE__H
