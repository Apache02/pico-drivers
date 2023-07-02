#ifndef LIB_DRIVERS_PWM_SERVO__H
#define LIB_DRIVERS_PWM_SERVO__H

#include "pico/stdio.h"
#include "Pwm.h"

class Servo {
private:
    Pwm pwm;

    float periodMs;

public:
    Servo(uint pin, float periodMs): pwm(pin), periodMs(periodMs) {};

    void init();

    uint32_t dutyMs(float ms);

    float dutyPercent(float percent);
};


#endif // LIB_DRIVERS_PWM_SERVO__H
