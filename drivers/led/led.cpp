#include "drivers/led.h"

void LED::set(bool value) {
    gpio_put(pin, value ^ inverted);
}

bool LED::get() {
    return (bool) gpio_get(pin) ^ inverted;
}

bool LED::toggle() {
    auto state = !get();
    set(state);
    return state;
}
