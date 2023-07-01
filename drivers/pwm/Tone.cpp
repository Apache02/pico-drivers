#include "Tone.h"
#include "hardware/gpio.h"
#include "hardware/pwm.h"
#include "hardware/clocks.h"

#define WRAP_VALUE 8192

void Tone::init_io() {
    gpio_init(pin);
    gpio_set_function(pin, GPIO_FUNC_PWM);
    sliceNum = pwm_gpio_to_slice_num(pin);
    pwm_set_wrap(sliceNum, WRAP_VALUE);
    pwm_set_gpio_level(pin, WRAP_VALUE >> 1);
}

void Tone::init() {
    init_io();
}

uint32_t Tone::freq(uint32_t frequency) {
    uint32_t procHz = clock_get_hz(clk_sys);
    float maxFreq = procHz / WRAP_VALUE;
    float divider = maxFreq / frequency;
    if (divider < 1.0f) {
        divider = 1.0f;
    } else if (divider > 256.0f) {
        divider = 256.0f;
    }

    pwm_set_clkdiv(sliceNum, divider);

    return (uint32_t) maxFreq / divider;
}

void Tone::set(bool isOn) {
    pwm_set_enabled(sliceNum, isOn);
}
