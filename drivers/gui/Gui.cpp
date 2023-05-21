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

void Gui::drawPixel(const Gui::Point p) {
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

void Gui::drawLine(const Gui::Point p1, const Gui::Point p2) {
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

void Gui::drawCircle(const Gui::Point center, int16_t r) {
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

void Gui::fillCircle(const Gui::Point center, int16_t r) {
    fillCircle(center.x, center.y, r);
}

void Gui::drawRect(int16_t x1, int16_t y1, int16_t x2, int16_t y2) {
    int x, xm, y, ym;

    if (x1 > x2) {
        x = x2;
        xm = x1;
    } else {
        x = x1;
        xm = x2;
    }
    if (y1 > y2) {
        y = y2;
        ym = y1;
    } else {
        y = y1;
        ym = y2;
    }

    for (; x <= xm; x++) {
        drawPixel(x, y1);
        drawPixel(x, y2);
    }
    for (; y <= ym; y++) {
        drawPixel(x1, y);
        drawPixel(x2, y);
    }
}

void Gui::drawRect(const Point p1, const Point p2) {
    drawRect(p1.x, p1.y, p2.x, p2.y);
}

void Gui::drawRect(const Gui::Rect rect) {
    drawRect(rect.x1, rect.y1, rect.x2, rect.y2);
}

void Gui::fillRect(int16_t x1, int16_t y1, int16_t x2, int16_t y2) {
    int x, xm, y0, ym;

    if (x1 > x2) {
        x = x2;
        xm = x1;
    } else {
        x = x1;
        xm = x2;
    }
    if (y1 > y2) {
        y0 = y2;
        ym = y1;
    } else {
        y0 = y1;
        ym = y2;
    }

    for (; x <= xm; x++) {
        for (auto y = y0; y <= ym; y++) {
            drawPixel(x, y);
        }
    }
}

void Gui::fillRect(const Point p1, const Point p2) {
    fillRect(p1.x, p1.y, p2.x, p2.y);
}

void Gui::fillRect(const Gui::Rect rect) {
    fillRect(rect.x1, rect.y1, rect.x2, rect.y2);
}

void Gui::setFont(const FONT_DEF *set_font) {
    font = set_font;
}

void Gui::drawGlyph(const wchar_t c, int16_t x0, int16_t y0) {
    if (!font) return;
    if (c < font->first || c > font->last) return;

    const uint8_t *glyph_buffer = font->buffer + (font->height * (c - font->first));

    for (unsigned int y = 0; y < font->height; y++) {
        for (unsigned int x = 0; x < font->width; x++) {
            if (glyph_buffer[y] & (1u << x)) {
                drawPixel(x0 + x, y0 + y);
            }
        }
    }
}

void Gui::drawGlyph(const wchar_t c, const Gui::Point p) {
    drawGlyph(c, p.x, p.y);
}

Gui::Point Gui::calcTextSize(const char *text, int16_t xspace, int16_t yspace) {
    if (!font) return {0, 0};

    int16_t x = 0, x_max = 0;
    int16_t y = 0;

    for (; *text != '\0'; text++) {
        if (*text == '\n') {
            x = 0;
            continue;
        }
        if (x == 0) {
            if (y > 0) {
                y += yspace;
            }
            y += font->height;
        } else if (x > 0) {
            x += xspace;
        }
        x += font->width;

        if (x > x_max) {
            x_max = x;
        }
    }

    return {x_max, y};
}

void Gui::drawText(const char *text, const Rect rect, int16_t xspace, int16_t yspace, Gui::Align align) {
    if (!font) return;

    Point size = calcTextSize(text, xspace, yspace);
    int16_t x = rect.x1;
    int16_t y = rect.y1;

    if (align & Align::RIGHT) {
        x += rect.x2 - size.x;
    } else if (align & Align::CENTER) {
        x += (rect.x2 - size.x) / 2;
    }

    if (align & Align::BOTTOM) {
        y += rect.y2 - size.y;
    } else if (align & Align::MIDDLE) {
        y += (rect.y2 - size.y) / 2;
    }

    for (const char *tmp = text; *tmp != '\0'; tmp++) {
        wchar_t c = *tmp;
        if (c == '\n') {
            x = 0;
            y += font->height + yspace;
            continue;
        }

        drawGlyph(c, x, y);
        x += font->width + xspace;
    }

}

void Gui::drawText(const char *text, const Gui::Rect rect, int16_t space, Gui::Align align) {
    drawText(text, rect, space, space, align);
}

