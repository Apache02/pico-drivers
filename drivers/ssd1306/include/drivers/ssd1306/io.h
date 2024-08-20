#ifndef DRIVERS_SSD1306_DEVICE_IO__H
#define DRIVERS_SSD1306_DEVICE_IO__H

#include <stdlib.h>
#include "pico/stdlib.h"
#include "drivers/I2C.h"
#include "drivers/SPI.h"


namespace ssd1306 {

    class DeviceIO {
    public:
        virtual void command(const uint8_t command) {};

        virtual void commands(const uint8_t *commands, size_t length) {};

        virtual void write(const uint8_t *data, size_t length) {};
    };


    class I2C : public DeviceIO {
    private:
        IO::AbstractI2C &iic;
        uint8_t address;

        void chunked_write(const uint8_t control_byte, const uint8_t *data, const size_t length);

    public:
        I2C(IO::AbstractI2C &iic, uint8_t address) : iic(iic), address(address) {};

        void command(const uint8_t command) override;

        void commands(const uint8_t *commands, size_t length) override;

        void write(const uint8_t *data, size_t length) override;
    };


    class SPI : public DeviceIO {
    private:
        IO::SPI &spi;

        uint cs;
        uint dc;

        void internal_write(const uint8_t *data, size_t length, bool is_data);

    public:
        SPI(IO::SPI &spi, uint cs, uint dc, uint reset);


        void command(const uint8_t command) override;

        void commands(const uint8_t *commands, size_t length) override;

        void write(const uint8_t *data, size_t length) override;
    };

}

#endif // DRIVERS_SSD1306_DEVICE_IO__H