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
#ifdef DEBUG
        printf("WS2817 init\n");
#endif
        // todo get free sm
        PIO pio = pio0;
        int sm = 0;
        uint offset = pio_add_program(pio, &ws2812_program);

        ws2812_program_init(pio, sm, offset, WS2812_PIN, 800000, IS_RGBW);
    }
    static inline void put_pixel(uint32_t pixel_grb) {
        pio_sm_put_blocking(pio0, 0, pixel_grb << 8u);
    }
    static inline uint32_t urgb_u32(uint8_t r, uint8_t g, uint8_t b) {
        return
                ((uint32_t) (r) << 8) |
                ((uint32_t) (g) << 16) |
                (uint32_t) (b);
    }
};

#endif
