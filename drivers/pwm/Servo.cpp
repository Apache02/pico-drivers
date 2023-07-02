#include "Servo.h"


void Servo::init() {
    pwm.init(UINT16_MAX, 1000 / periodMs);
    pwm.on();
}

uint32_t Servo::dutyMs(float ms) {
    if (ms > periodMs) {
        ms = periodMs;
    }

    uint16_t duty = UINT16_MAX * float(ms / periodMs);
    pwm.set_level(duty);

    return ms;
}

float Servo::dutyPercent(float percent) {
    if (percent > 100) {
        percent = 100;
    } else if (percent < 0) {
        percent = 0;
    }

    uint16_t duty = UINT16_MAX * float(percent / 100);
    pwm.set_level(duty);

    return percent;
}
