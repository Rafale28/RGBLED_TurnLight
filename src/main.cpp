#include <cstdint>
#include <cstdio>

#include "pico/stdlib.h"
#include "seq_turn_signal.h"

int main() {
    //set_sys_clock_48();
    stdio_init_all();
    printf("WS2812 Smoke Test, using pin %d", WS2812_PIN);

    int t = 0;
    TURN_SIGNAL ptn;
    ptn.startTimer();
    ptn.run(t);
}