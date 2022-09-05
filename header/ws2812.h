#ifndef __WS2812_H__
#define __WS2812_H__

#include <stdio.h>
#include <stdlib.h>

#include "pico/stdlib.h"
#include "ws2812.pio.h"

#define IS_RGBW true
#define NUM_PIXELS 8

#ifdef PICO_DEFAULT_WS2812_PIN
#define WS2812_PIN PICO_DEFAULT_WS2812_PIN
#else
// default to pin 2 if the board doesn't have a default WS2812 pin defined
#define WS2812_PIN 2
#endif

class WS2812 {
public:
    WS2812() {
        printf("WS2817 init\n");
        // todo get free sm
        PIO pio = pio0;
        int sm = 0;
        uint offset = pio_add_program(pio, &ws2812_program);

        ws2812_program_init(pio, sm, offset, WS2812_PIN, 800000, IS_RGBW);
    }
    static inline void put_pixel(uint32_t pixel_grb);
    static inline uint32_t urgb_u32(uint8_t r, uint8_t g, uint8_t b);
};

class RGB_PTN : public WS2812 {
public:

    void pattern_snakes(uint len, uint t);
    void pattern_random(uint len, uint t);
    void pattern_sparkle(uint len, uint t);
    void pattern_greys(uint len, uint t);

    typedef void (RGB_PTN::*pattern)(uint, uint);
    typedef struct pattern_t{
        pattern pat;
        const char *name;
    } PATTERN_T;
    PATTERN_T pattern_table[4] = {
            {&RGB_PTN::pattern_snakes,  "Snakes!"},
            {&RGB_PTN::pattern_random,  "Random data"},
            {&RGB_PTN::pattern_sparkle, "Sparkles"},
            {&RGB_PTN::pattern_greys,   "Greys"}
    };

    void run(int ptn, int t) {
        (this->*pattern_table[ptn].pat)(NUM_PIXELS, t);
    }
    int getPcount() {
        return count_of(pattern_table);
    }
    const char *getName(int ptn) {
        return this->pattern_table[ptn].name;
    }
};

#endif