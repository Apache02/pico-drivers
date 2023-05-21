#ifndef LIB_DRIVERS_FONT_DEF__H
#define LIB_DRIVERS_FONT_DEF__H

typedef struct {
    unsigned char width;
    unsigned char height;
    wchar_t first;
    wchar_t last;
    const uint8_t *buffer;
} FONT_DEF;

#endif // LIB_DRIVERS_FONT_DEF__H
