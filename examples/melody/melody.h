#pragma once

#include <stdio.h>
#include "drivers/Tone.h"

typedef struct {
    Tone::NOTE freq;
    int8_t divider;
} Note;
