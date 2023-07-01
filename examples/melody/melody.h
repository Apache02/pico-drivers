#pragma once

#include <stdio.h>
#include "drivers/pwm/Tone.h"

typedef struct {
    Tone::NOTE freq;
    int8_t divider;
} Note;
