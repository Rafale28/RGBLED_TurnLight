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


static inline void put_pixel(uint32_t pixel_grb);
static inline uint32_t urgb_u32(uint8_t r, uint8_t g, uint8_t b);

void pattern_snakes(uint len, uint t);

void pattern_random(uint len, uint t);

void pattern_sparkle(uint len, uint t);

void pattern_greys(uint len, uint t);

typedef void (*pattern)(uint, uint);
const struct {
    pattern pat;
    const char *name;
} pattern_table[] = {
        {pattern_snakes,  "Snakes!"},
        {pattern_random,  "Random data"},
        {pattern_sparkle, "Sparkles"},
        {pattern_greys,   "Greys"},
};

#endif