#include "drivers/lcd/display.h"

//  COMMAND             | 7 | 6 | 5 | 4 | 3 | 2 | 1 | 0 | flags
// -------------------------------------------------------------
// LCD_CLEARDISPLAY     |                             1 |
// LCD_RETURNHOME       |                         1   0 | no options
// LCD_ENTRYMODESET     |                     1   i   s | i - increment, s - shift
// LCD_DISPLAYCONTROL   |                 1   D   C   B | D - on, C - cursor, B - blink
// LCD_CURSORSHIFT      |             1  SC  RL   _   _ |
// LCD_FUNCTIONSET      |         1  DL   N   F   _   _ | DL - 8bits mode, N - 2 line display, F - font 5x11
// LCD_SETCGRAMADDR     |     1   A   A   A   A   A   A | A - address
// LCD_SETDDRAMADDR     | 1   A   A   A   A   A   A   A | A - address


// commands
#define LCD_CLEARDISPLAY            0x01
#define LCD_RETURNHOME              0x02
#define LCD_ENTRYMODESET            0x04
#define LCD_DISPLAYCONTROL          0x08
#define LCD_CURSORSHIFT             0x10
#define LCD_FUNCTIONSET             0x20
#define LCD_SETCGRAMADDR            0x40
#define LCD_SETDDRAMADDR            0x80

// flags for display entry mode
#define LCD_ENTRYSHIFTINCREMENT     0x01
#define LCD_ENTRYLEFT               0x02

// flags for display and cursor control
#define LCD_BLINKON                 0x01
#define LCD_CURSORON                0x02
#define LCD_DISPLAYON               0x04

// flags for display and cursor shift
#define LCD_MOVERIGHT               0x04
#define LCD_DISPLAYMOVE             0x08

// flags for function set
#define LCD_5x10DOTS                0x04
#define LCD_2LINE                   0x08
#define LCD_8BITMODE                0x10


namespace LCD {
    void Display::configure() {
        io.configure();

        io.command(LCD_FUNCTIONSET | (linesCount == 2 ? LCD_2LINE : 0)); // 8 bit
        io.command(LCD_DISPLAYCONTROL); // Display OFF
        sleep_us(2000);
        io.command(LCD_CLEARDISPLAY); // Clear display
        io.command(LCD_ENTRYMODESET | LCD_ENTRYLEFT); // entry mode set
        io.command(LCD_CURSORSHIFT | LCD_MOVERIGHT);
        sleep_us(2000);
    }

    void Display::clear() {
        io.command(LCD_CLEARDISPLAY);
        sleep_us(2000);
    }

    void Display::position(uint8_t line, uint8_t position) {
        io.command(LCD_SETDDRAMADDR | position | (line > 0 ? 0x40 : 0));
    }

    void Display::on(bool cursor = false, bool blink = false) {
        io.command(
                LCD_DISPLAYCONTROL
                | LCD_DISPLAYON
                | (cursor ? LCD_CURSORON : 0)
                | (cursor && blink ? LCD_BLINKON : 0)
        );
        sleep_us(2000);
    }

    void Display::off() {
        io.command(LCD_DISPLAYCONTROL);
        sleep_us(2000);
    }

    void Display::putChar(int c) {
        io.write(c);
    }

    void Display::print(const char *s) {
        while (*s != '\0') {
            io.write(*s++);
        }
    }
}
