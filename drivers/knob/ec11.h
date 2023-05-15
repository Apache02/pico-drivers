#ifndef DRIVERS_EC11__H
#define DRIVERS_EC11__H

#include "pico/stdlib.h"

#ifndef EC11_MAX_INSTANCES
#define EC11_MAX_INSTANCES 2
#endif

#ifndef EC11_COUNT_BEFORE_FAST
#define EC11_COUNT_BEFORE_FAST 2
#endif

#ifndef EC11_DELAY_BEFORE_FAST_MS
#define EC11_DELAY_BEFORE_FAST_MS 50
#endif

#ifndef EC11_DELAY_AFTER_FAST_MS
#define EC11_DELAY_AFTER_FAST_MS 400
#endif

class EC11 {
public:
    typedef enum {
        EMPTY = 0,
        ERROR,
        KEY_PRESS,
        KEY_RELEASE,
        KEY_LONG_PRESS,
        SIMPLE_CW,
        SIMPLE_CCW,
        FAST_CW,
        FAST_CCW,
    } EC11_EVENT_T;

    typedef void (EC11_callback_t)(EC11_EVENT_T signal);

private:
    uint s1;
    uint s2;
    int key;

    bool state_key = false;
    uint state_bits = 0;

    EC11_callback_t *_callback = nullptr;

    EC11_EVENT_T fast_event = EMPTY;
    uint32_t fast_count = 0;
    uint64_t fast_time_ms = 0;

    EC11_EVENT_T last_event = EMPTY;

    void init_internal();

    static void gpio_callback(uint gpio, uint32_t events);

    void process_callback(uint gpio, uint32_t events);

public:
    EC11(uint s1_pin, uint s2_pin, int key_pin = -1) : s1(s1_pin), s2(s2_pin), key(key_pin) {
        init_internal();
    };

    void register_callback(EC11_callback_t *callback);

    bool is_key_pressed();

    EC11_EVENT_T get_event();
};


#endif // DRIVERS_EC11__H
