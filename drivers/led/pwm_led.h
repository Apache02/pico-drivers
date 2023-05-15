#ifndef DRIVERS_LED_PWM_LED__H
#define DRIVERS_LED_PWM_LED__H

#include "hardware/gpio.h"
#include "hardware/pwm.h"


class PwmLED {
private:
    uint pin;
    bool inverted;
    uint8_t led_value;

    void update_pwm();

public:
    PwmLED(uint pin, bool inverted = false) : pin(pin), inverted(inverted) {
        gpio_init(pin);
        gpio_set_function(pin, GPIO_FUNC_PWM);

        pwm_config pwm_cfg = pwm_get_default_config();
        pwm_config_set_wrap(&pwm_cfg, UINT16_MAX);
        pwm_init(pwm_gpio_to_slice_num(pin), &pwm_cfg, true);

        set(0);
    };

    ~PwmLED() {
        gpio_set_function(pin, GPIO_FUNC_NULL);
    }

    void set(uint8_t level);

    inline void on() {
        set(UINT8_MAX);
    };

    inline void off() {
        set(0);
    };
};


#endif // DRIVERS_LED_PWM_LED__H
