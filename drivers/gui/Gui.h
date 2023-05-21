#ifndef LIB_DRIVERS_GUI__H
#define LIB_DRIVERS_GUI__H

#include "pico/stdlib.h"
#include "../fonts/font_def.h"


class Gui {
public:
    typedef struct {
        int16_t x;
        int16_t y;
    } Point;

    typedef struct {
        int16_t x1;
        int16_t y1;
        int16_t x2;
        int16_t y2;
    } Rect;

    typedef enum {
        LEFT = 0b0000,
        RIGHT = 0b0001,
        CENTER = 0b0010,

        TOP = 0b0000,
        BOTTOM = 0b0100,
        MIDDLE = 0b1000,
    } Align;

    const uint16_t width;
    const uint16_t height;

private:
    uint8_t *buffer;
    const FONT_DEF *font = nullptr;

public:
    Gui(uint8_t *buffer, uint16_t width, uint16_t height) : buffer(buffer), width(width), height(height) {};

    void fill(uint32_t color);

    void drawPixel(int16_t x, int16_t y);

    void drawPixel(const Point p);

    void drawLine(int16_t x1, int16_t y1, int16_t x2, int16_t y2);

    void drawLine(const Point p1, const Point p2);

    void drawCircle(int16_t x0, int16_t y0, int16_t r);

    void drawCircle(const Point center, int16_t r);

    void fillCircle(int16_t x0, int16_t y0, int16_t r);

    void fillCircle(const Point center, int16_t r);

    void drawRect(int16_t x1, int16_t y1, int16_t x2, int16_t y2);

    void drawRect(const Point p1, const Point p2);

    void drawRect(const Rect rect);

    void fillRect(int16_t x1, int16_t y1, int16_t x2, int16_t y2);

    void fillRect(const Point p1, const Point p2);

    void fillRect(const Rect rect);

    void setFont(const FONT_DEF *font);

    void drawGlyph(const wchar_t c, int16_t x, int16_t y);

    void drawGlyph(const wchar_t c, const Point p);

    Point calcTextSize(const char *text, int16_t xspace = 0, int16_t yspace = 0);

    void drawText(const char *text, const Rect rect, int16_t xspace, int16_t yspace, Align align = Align::LEFT);

    void drawText(const char *text, const Rect rect, int16_t space, Align align = Align::LEFT);
};

#endif // LIB_DRIVERS_GUI__H
