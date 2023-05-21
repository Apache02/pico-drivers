#include "pico/stdlib.h"
#include "fonts.h"

#include "font_5x8.h"

#define buffer_ptr(buf) (const uint8_t *) &buf[0]

const FONT_DEF font_5x8 = {
        5,
        8,
        '\0',
        '\0' + count_of(font_5x8_buffer) - 1,
        buffer_ptr(font_5x8_buffer)
};
