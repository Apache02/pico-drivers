#include "Gui.h"
#include <string.h>

void Gui::fill(uint32_t color) {
    memset(buffer, color ? 0xFF : 0x00, width * (height >> 3));
}

void Gui::drawPixel(int16_t x, int16_t y) {
    if ((x < 0) || (x >= width) || (y < 0) || (y >= height)) {
        return;
    }

    buffer[x + (y >> 3) * width] |= 1u << (y & 0b111);
}

void Gui::drawPixel(Gui::Point p) {
    drawPixel(p.x, p.y);
}

void Gui::drawLine(int16_t x1, int16_t y1, int16_t x2, int16_t y2) {
    int n, dx, dy, sgndx, sgndy, dxabs, dyabs, x, y, drawx, drawy;

    dx = x2 - x1;
    dy = y2 - y1;
    dxabs = (dx > 0) ? dx : -dx;
    dyabs = (dy > 0) ? dy : -dy;
    sgndx = (dx > 0) ? 1 : -1;
    sgndy = (dy > 0) ? 1 : -1;
    x = dyabs >> 1;
    y = dxabs >> 1;
    drawx = x1;
    drawy = y1;

    drawPixel(drawx, drawy);

    if (dxabs >= dyabs) {
        for (n = 0; n < dxabs; n++) {
            y += dyabs;
            if (y >= dxabs) {
                y -= dxabs;
                drawy += sgndy;
            }
            drawx += sgndx;
            drawPixel(drawx, drawy);
        }
    } else {
        for (n = 0; n < dyabs; n++) {
            x += dxabs;
            if (x >= dyabs) {
                x -= dyabs;
                drawx += sgndx;
            }
            drawy += sgndy;
            drawPixel(drawx, drawy);
        }
    }
}

void Gui::drawLine(Gui::Point p1, Gui::Point p2) {
    drawLine(p1.x, p1.y, p2.x, p2.y);
}

void Gui::drawCircle(int16_t x0, int16_t y0, int16_t r) {
    if (r < 0) {
        return;
    }

    int x = r;
    int y = 0;
    int de = 1 - x;

    while (x >= y) {
        drawPixel(x + x0, y + y0);
        drawPixel(y + x0, x + y0);
        drawPixel(-y + x0, x + y0);
        drawPixel(-x + x0, y + y0);
        drawPixel(-x + x0, -y + y0);
        drawPixel(-y + x0, -x + y0);
        drawPixel(y + x0, -x + y0);
        drawPixel(x + x0, -y + y0);
        y++;
        if (de <= 0) {
            de += 2 * y + 1;
        } else {
            x--;
            de += 2 * (y - x) + 1;
        }
    }
}

void Gui::drawCircle(Gui::Point center, int16_t r) {
    drawCircle(center.x, center.y, r);
}

void Gui::fillCircle(int16_t x0, int16_t y0, int16_t r) {
    int16_t x, y, xd;

    if (r <= 0) return;

    xd = 3 - (r << 1);
    x = 0;
    y = r;

    while (x <= y) {
        if (y > 0) {
            drawLine(x0 - x, y0 - y, x0 - x, y0 + y);
            drawLine(x0 + x, y0 - y, x0 + x, y0 + y);
        }
        if (x > 0) {
            drawLine(x0 - y, y0 - x, x0 - y, y0 + x);
            drawLine(x0 + y, y0 - x, x0 + y, y0 + x);
        }
        if (xd < 0) {
            xd += (x << 2) + 6;
        } else {
            xd += ((x - y) << 2) + 10;
            y--;
        }
        x++;
    }
    drawCircle(x0, y0, r);
}

void Gui::fillCircle(Gui::Point center, int16_t r) {
    fillCircle(center.x, center.y, r);
}

void Gui::drawRect(int16_t x1, int16_t y1, int16_t x2, int16_t y2) {
    drawLine(x1, y1, x2, y1);
    drawLine(x1, y2, x2, y2);
    drawLine(x1, y1, x1, y2);
    drawLine(x2, y1, x2, y2);
}

void Gui::drawRect(Gui::Rect rect) {
    drawRect(rect.x1, rect.y1, rect.x2, rect.y2);
}

