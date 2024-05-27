#include <stdio.h>
#include "pico/stdlib.h"
#include "pico/time.h"
#include "pico/binary_info.h"
#include <map>
#include <string>

#include "drivers/led.h"
#include "drivers/Tone.h"

// board config
#define LED_PIN PICO_DEFAULT_LED_PIN
#define BUZZER_PIN 6
#define TIME_UNIT_DURATION 100

// global variables
LED led(LED_PIN);
Tone buzzer(BUZZER_PIN);

const char message[] = "Morse code is a method used in telecommunication to encode text "
                       "characters as standardized sequences of two different signal durations, "
                       "called dots and dashes, or dits and dahs. Morse code is named after "
                       "Samuel Morse, one of the inventors of the telegraph.";

const std::map<const char, const char *> morzeMap = {
        {'a', ".-"},
        {'b', "-..."},
        {'w', ".--"},
        {'g', "--."},
        {'d', "-.."},
        {'e', "."},
        {'v', "...-"},
        {'z', "--.."},
        {'i', ".."},
        {'j', ".---"},
        {'k', "-.-"},
        {'l', ".-.."},
        {'m', "--"},
        {'n', "-."},
        {'o', "---"},
        {'p', ".--."},
        {'r', ".-."},
        {'s', "..."},
        {'t', "-"},
        {'u', "..-"},
        {'f', "..-."},
        {'h', "...."},
        {'c', "-.-."},
        {'q', "--.-"},
        {'y', "-.--"},
        {'x', "-..-"},
        {'1', ".----"},
        {'2', "..---"},
        {'3', "...--"},
        {'4', "....-"},
        {'5', "....."},
        {'6', "-...."},
        {'7', "--..."},
        {'8', "---.."},
        {'9', "----."},
        {'0', "-----"},
        {'.', "......"},
        {',', ".-.-.-"},
        {':', "---..."},
        {';', "-.-.-."},
        {'(', "-.--.-"},
        {')', "-.--.-"},
        {'"', ".-..-."},
        {'-', "-....-"},
        {'/', "-..-."},
        {'?', "..--.."},
        {'!', "--..--"},
        {' ', "-...-"},
        {'@', ".--.-."},
};

void startup_test() {
    printf("%s START\n", __FUNCTION__);
    led.on();
    sleep_ms(2000);
    led.off();
    sleep_ms(10);
    printf("%s DONE\n", __FUNCTION__);
}

int main() {
    bi_decl(bi_program_description("This is a pico-drivers-example binary."));
    bi_decl(bi_1pin_with_name(LED_PIN, "On-board LED"));

    stdio_init_all();
    buzzer.init();

    startup_test();

    buzzer.freq(1100);

    char c;
    auto ptr = &message[0];
    while ((c = *ptr++) != '\0') {
        c = tolower(c);
        auto item = morzeMap.find(c);

        if (item != morzeMap.end()) {
            printf("%c = %s\n", item->first, item->second);
            for (const char *tmp = item->second; *tmp != '\0'; tmp++) {
                buzzer.on();
                if (*tmp == '.') {
                    sleep_ms(TIME_UNIT_DURATION);
                } else if (*tmp == '-') {
                    sleep_ms(TIME_UNIT_DURATION * 3);
                }
                buzzer.off();
                sleep_ms(TIME_UNIT_DURATION);
            }
        }

        sleep_ms(TIME_UNIT_DURATION * 2);
    }

    for (;;) {
        tight_loop_contents();
    }

    return 0;
}
