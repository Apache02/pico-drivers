#include "FpsMeasure.h"


void FpsMeasure::reset() {
    start_ms = now_ms();
    frame_count = 0;
}

void FpsMeasure::frame() {
    frame_count++;
    uint64_t diff = now_ms() - start_ms;
    if (diff >= 1000) {
        fps = frame_count;
        reset();
    }
}

int FpsMeasure::getFps() {
    return fps;
}
