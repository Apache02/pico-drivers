#include "ec11.h"
#include <stdio.h>
#include "hardware/gpio.h"

// S1 ... 1[0011] ... 1[1001]
// S2 ... 1[1001] ... 1[0011]
//             ^           ^
//            CW          CCW

static EC11 *instances[EC11_MAX_INSTANCES] = {nullptr};

void EC11::gpio_callback(uint gpio, uint32_t events) {
    for (auto instance: instances) {
        if (instance && (instance->s1 == gpio || instance->s2 == gpio || instance->key == gpio)) {
            instance->process_callback(gpio, events);
            return;
        }
    }
}

void EC11::init_internal() {
    bool found = false;
    for (auto i = 0; i < EC11_MAX_INSTANCES; i++) {
        if (instances[i] == nullptr) {
            instances[i] = this;
            found = true;
            break;
        }
    }

    if (!found) {
#if PICO_CXX_ENABLE_EXCEPTIONS
        // TODO: throw exception
#endif
        return;
    }

    gpio_init(s1);
    gpio_init(s2);
    gpio_init(key);
    gpio_set_dir(s1, GPIO_IN);
    gpio_set_dir(s2, GPIO_IN);
    gpio_set_dir(key, GPIO_IN);
    gpio_set_irq_enabled_with_callback(s1, GPIO_IRQ_EDGE_RISE | GPIO_IRQ_EDGE_FALL, true, &EC11::gpio_callback);
    gpio_set_irq_enabled_with_callback(s2, GPIO_IRQ_EDGE_RISE | GPIO_IRQ_EDGE_FALL, true, &EC11::gpio_callback);
    gpio_set_irq_enabled_with_callback(key, GPIO_IRQ_EDGE_RISE | GPIO_IRQ_EDGE_FALL, true, &EC11::gpio_callback);

    state_bits = 0;
}

void EC11::process_callback(uint gpio, uint32_t events) {
    auto event = EMPTY;

    if (gpio == key) {
        fast_count = 0;

        if (events == GPIO_IRQ_EDGE_FALL) {
            state_key = true;
            event = KEY_PRESS;
        } else if (events == GPIO_IRQ_EDGE_RISE) {
            state_key = false;
            event = KEY_RELEASE;
        } else {
            state_key = !gpio_get(key);
            return;
//            event = ERROR;
        }

        last_event = event;
        if (_callback) {
            _callback(event);
        }

        return;
    }

    uint current_state = 0;
    current_state |= gpio_get(s1) & 1;
    current_state <<= 1;
    current_state |= gpio_get(s2) & 1;

    if (current_state != 0b11) {
        state_bits <<= 2;
        state_bits |= current_state;
        return;
    }

    current_state = state_bits;
    state_bits = 0;

    // end of cycle
    if (current_state == 0b010010) {
        event = SIMPLE_CW;
    } else if (current_state == 0b100001) {
        event = SIMPLE_CCW;
    } else {
        return;
//        event = EC11::ERROR;
    }

    // fast logic
    uint64_t time_ms = time_us_64() / 1000;
    if (
            fast_event == event
            && (
                    ((fast_count > EC11_COUNT_BEFORE_FAST) && (time_ms - fast_time_ms < EC11_DELAY_AFTER_FAST_MS)) ||
                    time_ms - fast_time_ms < EC11_DELAY_BEFORE_FAST_MS
            )
            ) {
        if (++fast_count > EC11_COUNT_BEFORE_FAST) {
            if (event == SIMPLE_CW) {
                event = FAST_CW;
            } else if (event == SIMPLE_CCW) {
                event = FAST_CCW;
            }
        }
    } else {
        fast_event = event;
        fast_count = 1;
    }
    fast_time_ms = time_ms;

    last_event = event;
    if (_callback) {
        _callback(event);
    }
}

void EC11::register_callback(EC11_callback_t *callback) {
    _callback = callback;
}

bool EC11::is_key_pressed() {
    return state_key;
}

EC11::EC11_EVENT_T EC11::get_event() {
    auto event = last_event;
    last_event = EMPTY;
    return event;
}
