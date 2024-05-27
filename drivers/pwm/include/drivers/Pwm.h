#ifndef LIB_DRIVERS_PWM_PWM__H
#define LIB_DRIVERS_PWM_PWM__H

#include "pico/stdio.h"

class Pwm {
private:
    uint pin;
    uint sliceNum;
    uint16_t wrapValue;

    void init_io();

public:
    Pwm(uint pin) : pin(pin){};

    void init(uint16_t sliceWrap = UINT16_MAX, uint32_t freq = 1000);

    void set_wrap(uint16_t wrap, uint16_t level = 0);

    uint32_t set_freq(uint32_t freq);

    void set_level(uint16_t level);

    void status(bool isOn);

    inline void on() {
        status(true);
    };

    inline void off() {
        status(false);
    };
};


#endif // LIB_DRIVERS_PWM_PWM__H
