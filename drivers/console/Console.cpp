#include "drivers/Console.h"
#include <string.h>
#include "standart_6x8_font.h"

#include <stdio.h>
#include <stdarg.h>

static struct {
    const uint8_t *glyphs;
    char first;
    char last;
    unsigned char width;
    int space;
} font = {
        (const uint8_t *) (standard_6x8_ascii_buffer),
        ' ',
        ' ' + count_of(standard_6x8_ascii_buffer) - 1,
        6,
        0,
};

void Console::clear() {
    memset(buffer, 0, width * lines_count);
    offset_x = 0;
    offset_line = 0;
}

void Console::drawSymbol(const char c, uint16_t x, uint8_t line) {
    if (c < font.first || c > font.last) return;
    if (line >= lines_count) return;

    memcpy(&buffer[(line * width) + x], &font.glyphs[(c - font.first) * font.width], font.width);
}

void Console::printChar(const char c) {
    if (c == '\n') {
        offset_x = 0;
        offset_line++;
    } else if (offset_x + font.width > width) {
        offset_x = 0;
        offset_line++;
    }

    if (offset_line >= lines_count) {
        memcpy(&buffer[0], &buffer[1 * width], (lines_count - 1) * width);
        memset(&buffer[(lines_count - 1) * width], 0x00, 1 * width);
        offset_line--;
    }

    if (c == '\n') {
        return;
    }

    drawSymbol(c, offset_x, offset_line);
    offset_x += font.width + font.space;
}

void Console::print(const char *text) {
    for (const char *ptr = text; *ptr != '\0'; ptr++) {
        printChar(*ptr);

        if (offset_line >= lines_count) {
            break;
        }
    }
}

int Console::printf(const char *format, ...) {
    va_list args;
    va_start (args, format);

    char tmp[256];
    tmp[sizeof(tmp) - 1] = 0;
    int c = vsnprintf(tmp, sizeof(tmp) - 1, format, args);
    print(tmp);

    va_end (args);
    return c;
}

void Console::printLine(const char *text) {
    if (offset_x) {
        printChar('\n');
    }
    print(text);
    printChar('\n');
}

void Console::clearLine() {
    memset(&buffer[offset_line * width], 0x00, 1 * width);
    offset_x = 0;
}
