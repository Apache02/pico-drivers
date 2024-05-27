#ifndef DRIVERS_LED_RGB_PWM_LED__H
#define DRIVERS_LED_RGB_PWM_LED__H

#include "hardware/gpio.h"
#include "hardware/pwm.h"

class RGBLED {

private:
    uint pin_r;
    uint pin_g;
    uint pin_b;
    bool inverted;
    pwm_config pwm_cfg;

    uint8_t led_brightness = 127;
    uint8_t led_r = 0;
    uint8_t led_g = 0;
    uint8_t led_b = 0;

    void update_pwm();

public:
    RGBLED(uint r, uint g, uint b, bool inverted = false)
            : pin_r(r), pin_g(g), pin_b(b), inverted(inverted) {
        gpio_init(r);
        gpio_init(g);
        gpio_init(b);
        gpio_set_function(r, GPIO_FUNC_PWM);
        gpio_set_function(g, GPIO_FUNC_PWM);
        gpio_set_function(b, GPIO_FUNC_PWM);

        pwm_cfg = pwm_get_default_config();
        pwm_config_set_wrap(&pwm_cfg, UINT16_MAX);
        pwm_init(pwm_gpio_to_slice_num(r), &pwm_cfg, true);
        pwm_init(pwm_gpio_to_slice_num(g), &pwm_cfg, true);
        pwm_init(pwm_gpio_to_slice_num(b), &pwm_cfg, true);
    };

    ~RGBLED() {
        gpio_set_function(pin_r, GPIO_FUNC_NULL);
        gpio_set_function(pin_g, GPIO_FUNC_NULL);
        gpio_set_function(pin_b, GPIO_FUNC_NULL);
    }

    void set_brightness(uint8_t brightness);

    void set_rgb(uint8_t r, uint8_t g, uint8_t b);
    void set_rgb(float r, float g, float b);
    void set_rgb(uint32_t color888);

    void set_hsv(float h, float s, float v);
};


#endif //DRIVERS_LED_RGB_PWM_LED__H
