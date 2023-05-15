#include "pwm_led.h"

void PwmLED::set(uint8_t val) {
    led_value = val;
    update_pwm();
}

void PwmLED::update_pwm() {
    uint16_t val = led_value * led_value;
    if (inverted) {
        val = UINT16_MAX - val;
    }
    pwm_set_gpio_level(pin, val);
}
