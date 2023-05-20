#ifndef LIB_DRIVERS_GUI__H
#define LIB_DRIVERS_GUI__H

#include "pico/stdlib.h"


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

private:
    uint8_t *buffer;
    uint16_t width;
    uint16_t height;

public:
    Gui(uint8_t *buffer, uint16_t width, uint16_t height) : buffer(buffer), width(width), height(height) {};

    void fill(uint32_t color);

    void drawPixel(int16_t x, int16_t y);

    void drawPixel(Point p);

    void drawLine(int16_t x1, int16_t y1, int16_t x2, int16_t y2);

    void drawLine(Point p1, Point p2);

    void drawCircle(int16_t x0, int16_t y0, int16_t r);

    void drawCircle(Point center, int16_t r);

    void fillCircle(int16_t x0, int16_t y0, int16_t r);

    void fillCircle(Point center, int16_t r);

    void drawRect(int16_t x1, int16_t y1, int16_t x2, int16_t y2);

    void drawRect(Rect rect);

//    void fillRect(Rect rect, uint32_t color);
};


#endif // LIB_DRIVERS_GUI__H
