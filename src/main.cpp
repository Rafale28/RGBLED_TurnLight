#include <cstdint>
#include <cstdio>

#include "pico/stdlib.h"
//#include "ws2812.h"
#include "seq_turn_signal.h"

int main() {
    //set_sys_clock_48();
    stdio_init_all();
    printf("WS2812 Smoke Test, using pin %d", WS2812_PIN);

    int t = 0;
    TURN_SIGNAL ptn;
    while (1) {
        int pat = rand() % ptn.getPcount();
        int dir = (rand() >> 30) & 1 ? 1 : -1;

        puts(ptn.getName(pat));
        puts(dir == 1 ? "(forward)" : "(backward)");

        for (int i = 0; i < 1000; ++i) {
            ptn.run(pat, t);
            sleep_ms(10);
            t += dir;
        }
    }
}