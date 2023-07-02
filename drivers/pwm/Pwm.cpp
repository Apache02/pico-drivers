#include "Pwm.h"
#include "hardware/gpio.h"
#include "hardware/pwm.h"
#include "hardware/clocks.h"

#define WRAP_VALUE 8192

void Pwm::init_io() {
    gpio_init(pin);
    gpio_set_function(pin, GPIO_FUNC_PWM);
    sliceNum = pwm_gpio_to_slice_num(pin);
}

void Pwm::set_wrap(uint16_t wrap, uint16_t level) {
    wrapValue = wrap;
    pwm_set_wrap(sliceNum, wrapValue);
    pwm_set_gpio_level(pin, level);
}

uint32_t Pwm::set_freq(uint32_t freq) {
    uint32_t procHz = clock_get_hz(clk_sys);
    float maxFreq = procHz / wrapValue;
    float divider = maxFreq / freq;
    if (divider < 1.0f) {
        divider = 1.0f;
    } else if (divider > 256.0f) {
        divider = 256.0f;
    }

    pwm_set_clkdiv(sliceNum, divider);

    return (uint32_t) maxFreq / divider;
}

void Pwm::init(uint16_t wrap, uint32_t freq) {
    init_io();
    set_wrap(wrap);
    set_freq(freq);
}

void Pwm::set_level(uint16_t level) {
    pwm_set_gpio_level(pin, level);
}

void Pwm::status(bool isOn) {
    pwm_set_enabled(sliceNum, isOn);
}
