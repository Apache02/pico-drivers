#include "pico/stdlib.h"
#include "drivers/fonts.h"

#include "font_6x8_basic.h"

#include "font_5x8.h"
#include "font_6x8.h"
#include "font_6x10.h"
#include "font_10x16.h"
#include "font_12x16.h"
#include "font_12x20.h"
#include "font_16x26.h"


#define buffer_ptr(buf) (const uint8_t *) &buf[0]
#define FONT_STRUCT(w, h, first, buffer) \
    { w, h, first, first + count_of(buffer) - 1, buffer_ptr(buffer) }

const FONT_DEF font_6x8_basic = FONT_STRUCT(6, 8, ' ', font_6x8_basic_buffer);

const FONT_DEF font_5x8 = FONT_STRUCT(5, 8, '\0', font_5x8_buffer);
const FONT_DEF font_6x8 = FONT_STRUCT(6, 8, '\0', font_6x8_buffer);
const FONT_DEF font_6x10 = FONT_STRUCT(6, 10, '\0', font_6x10_buffer);
const FONT_DEF font_10x16 = FONT_STRUCT(10, 16, '\0', font_10x16_buffer);
const FONT_DEF font_12x16 = FONT_STRUCT(12, 16, '\0', font_12x16_buffer);
const FONT_DEF font_12x20 = FONT_STRUCT(12, 20, '\0', font_12x20_buffer);
const FONT_DEF font_16x26 = FONT_STRUCT(16, 26, '\0', font_16x26_buffer);
