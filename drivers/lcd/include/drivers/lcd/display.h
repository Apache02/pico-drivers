#ifndef DRIVERS_LCD_DISPLAY__H
#define DRIVERS_LCD_DISPLAY__H

#include <stdlib.h>
#include "pico/stdlib.h"

#include "io.h"

namespace LCD {

    class Display {
    private:
        DeviceIO &io;
        uint8_t lineLength;
        uint8_t linesCount;

    public:
        Display(DeviceIO &io) : io(io) {
            linesCount = 1;
            lineLength = 16;
        };

        Display(DeviceIO &io, uint8_t lines, uint8_t length) : io(io), linesCount(lines), lineLength(length) {};

        void configure();

        void on(bool cursor, bool blink);

        void off();

        void position(uint8_t line, uint8_t position);

        void clear(void);

        void putChar(int c);

        void print(const char *s);
    };
}

#endif // DRIVERS_LCD_DISPLAY__H