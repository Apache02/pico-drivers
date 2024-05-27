#ifndef DRIVERS_LED__H
#define DRIVERS_LED__H

#include "hardware/gpio.h"


class LED {
private:
    uint pin;
    bool inverted;

public:
    LED(uint pin, bool inverted = false) : pin(pin), inverted(inverted) {
        gpio_init(pin);
        gpio_set_dir(pin, GPIO_OUT);
    };

    ~LED() {
        gpio_set_function(pin, GPIO_FUNC_NULL);
    }

    void set(bool value);

    bool get();

    bool toggle();

    inline void on() {
        set(true);
    };

    inline void off() {
        set(false);
    }
};

#endif // DRIVERS_LED__H
