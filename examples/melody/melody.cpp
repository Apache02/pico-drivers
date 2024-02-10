#include <stdio.h>
#include "pico/stdlib.h"
#include "pico/time.h"
#include "pico/binary_info.h"

#include "drivers/led/led.h"
#include "drivers/pwm/Tone.h"

#include "melody.h"
#include "mario_bros.h"
#include "pirates.h"

// board config
#define LED_PIN PICO_DEFAULT_LED_PIN
#define BUZZER_PIN 6

// global variables
LED led(LED_PIN);
Tone buzzer(BUZZER_PIN);

const Note example[] = {
        {Tone::NOTE::C4,   4},
        {Tone::NOTE::G3,   8},
        {Tone::NOTE::G3,   8},
        {Tone::NOTE::A3,   4},
        {Tone::NOTE::G3,   4},
        {Tone::NOTE::NONE, 4},
        {Tone::NOTE::B3,   4},
        {Tone::NOTE::C4,   4},
};

void startup_test() {
    printf("%s START\n", __FUNCTION__);
    led.on();
    sleep_ms(2000);
    led.off();
    sleep_ms(10);
    printf("%s DONE\n", __FUNCTION__);
}

void play_melody(const Note *notes, size_t length, uint tempo = 120) {
    for (auto i = 0; i < length; i++) {
        auto note = notes[i];
        auto freq = note.freq;
        auto divider = note.divider;
        bool extraHalfDuration = divider < 0;
        if (extraHalfDuration) {
            divider *= -1;
        }
        auto duration = 60 * 1000 * 2 / tempo / note.divider;
        if (extraHalfDuration) {
            duration += duration >> 1;
        }

        if (freq == 0) {
            buzzer.off();
        } else {
            buzzer.freq(note.freq);
            buzzer.on();
            led.on();
        }

        uint32_t noteDuration = duration * 0.9;
        sleep_ms(noteDuration);
        buzzer.off();
        led.off();
        sleep_ms(duration - noteDuration);
    }
}

int main() {
    bi_decl(bi_program_description("This is a pico-drivers-example binary."));
    bi_decl(bi_1pin_with_name(LED_PIN, "On-board LED"));

    stdio_init_all();
    buzzer.init();

    startup_test();

    buzzer.off();

    static const struct {
        const char *title;
        uint tempo;
        const Note *notes;
        const size_t length;
    } melodies[] = {
            {"example", 120, example,    count_of(example)},
            {"mario",   120, mario_bros, count_of(mario_bros)},
            {"pirates", 120, pirates,    count_of(pirates)},
    };

    for (auto[title, tempo, notes, length]: melodies) {
        printf("play melody [%s] at tempo %d bpm\n", title, tempo);
        play_melody(notes, length, tempo);
        sleep_ms(1000);
    }

    for (;;) {
        tight_loop_contents();
    }

    return 0;
}
