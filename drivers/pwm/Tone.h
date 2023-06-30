#ifndef LIB_DRIVERS_PWM_TONE__H
#define LIB_DRIVERS_PWM_TONE__H

#include "pico/stdio.h"


class Tone {
    typedef enum {
        TEST = 0,
    } TONE;

private:
    uint pin;
    uint sliceNum;

    void init_io();

public:
    Tone(uint pin) : pin(pin) {};

    void init();

    uint32_t freq(uint32_t frequency);

//    void play(TONE tone, uint64_t duration);

    void on();

    void off();
};


#endif // LIB_DRIVERS_PWM_TONE__H
