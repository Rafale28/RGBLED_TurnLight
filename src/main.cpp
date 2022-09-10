#include <cstdint>
#include <cstdio>

#include "pico/stdlib.h"
#include "seq_turn_signal.h"

int main() {
    //set_sys_clock_48();
    stdio_init_all();
    TURN_SIGNAL turn_signal;
    printf("RGB LED TURN LIGHT FOR YZF-R6 \n");
    printf("FW VERSION : %s\n", FW_VERSION);

    turn_signal.run();
}