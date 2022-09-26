#include "seq_turn_signal.h"

void TURN_SIGNAL::pattern_snakes(PATTERN_PARAM_T *pp_p) {
    int len = led_info.num_pixels;
    static int t = 0;
    for (uint i = 0; i < len; ++i) {
        uint x = (i + (t >> 1)) % 64;
        if (x < 10)
            put_pixel(urgb_u32(0xff, 0, 0));
        else if (x >= 15 && x < 25)
            put_pixel(urgb_u32(0, 0xff, 0));
        else if (x >= 30 && x < 40)
            put_pixel(urgb_u32(0, 0, 0xff));
        else
            put_pixel(0);
    }
    t++;
}

void TURN_SIGNAL::pattern_random(PATTERN_PARAM_T *pp_p) {
    int len = led_info.num_pixels;
    for (int i = 0; i < len; ++i)
        put_pixel(rand());
}

void TURN_SIGNAL::pattern_sparkle(PATTERN_PARAM_T *pp_p) {
    int len = led_info.num_pixels;
    int t = 0;
    if (t % 8)
        return;
    for (int i = 0; i < len; ++i)
        put_pixel(rand() % 16 ? 0 : 0xffffffff);
}


void TURN_SIGNAL::pattern_greys(PATTERN_PARAM_T *pp_p) {
    int len = led_info.num_pixels;
    int t = 0;
    int max = 100; // let's not draw too much current!
    t %= max;
    for (int i = 0; i < len; ++i) {
        put_pixel(t * 0x10101);
        if (++t >= max) t = 0;
    }
}

void TURN_SIGNAL::pattern_rwave(PATTERN_PARAM_T *pp_p) {
    int len = led_info.num_pixels;
    RGB32_t rLine[len] = {0};
    for (int d = 0; d < 180 + (10 * len); d += 5 ){
        float f[len] = {0};
        printf("Duty %.2f\n", f);
        for (int i =0; i < len; i++) {
            int di = 0;
            di = ((d - i*10) < 180) ? d - i * 10 : 180;
            if (di < 0) di = 0;
            f[i] = sinCalc(di);
            rLine[i] = setRGB((uint8_t)(f[i] * 0xff), (uint8_t)(f[i] * 0x6A), 0x00);
        }
        for (int i =0; i < len; i++) {
            put_pixel(urgb_u32(rLine[i]));
        }
        sleep_ms(30);
    }
}

void TURN_SIGNAL::pattern_p1_sin(PATTERN_PARAM_T *pp_p) {
    int len = led_info.num_pixels;
    RGB32_t rLine[len] = {0};
    int max_dig = 90;//max digree
    int p_diff = 2; //phase difference
    float f[len] = {0};
    for (int d = 0; d < max_dig + (p_diff * len); d += 5 ){
        for (int i =0; i < len; i++) {
            int di = 0;
            di = ((d - i*p_diff) < max_dig) ? d - i * p_diff : max_dig;
            if (di < 0) di = 0;
            f[i] = sinCalc(di);
            rLine[i] = setRGB((uint8_t)(f[i] * 0xff), (uint8_t)(f[i] * 0x6A), 0x00);
        }
        for (int i =len -1; i >= 0; i--) {
            put_pixel(urgb_u32(rLine[i]));
        }
#if LED_DEBUG
        printf("Duty[0] %.2f, Duty[%d] %.2f\n", f[0], len - 1, f[len - 1]);
#endif
        sleep_ms(30);
    }
    for (int d = max_dig; d >= 0 ; d -= 5 ){
        if (d < 0) d=0;
        f[0] = sinCalc(d);
        printf("Duty[0] %.2f\n", f[0]);
        rLine[0] = setRGB((uint8_t)(f[0] * 0xff), (uint8_t)(f[0] * 0x6A), 0x00);
        for (int i =0; i < len; i++) {
            put_pixel(urgb_u32(rLine[0]));
        }
        sleep_ms(20);
    }
}

void TURN_SIGNAL::pattern_p2_sin(PATTERN_PARAM_T *pp_p) {
    int len = led_info.num_pixels;
    RGB32_t rLine[len] = {0};
    int max_dig = 180;//max digree
    int p_diff = 2; //phase difference
    float f[len] = {0};
    for (int d = 0; d < max_dig + (p_diff * len); d += 5 ){
        for (int i =0; i < len; i++) {
            int di = 0;
            di = ((d - i*p_diff) < max_dig) ? d - i * p_diff : max_dig;
            if (di < 0) di = 0;
            f[i] = sinCalc(di);
            rLine[i] = setRGB((uint8_t)(f[i] * 0xff), (uint8_t)(f[i] * 0x6A), 0x00);
        }
        for (int i =len -1; i >= 0; i--) {
            put_pixel(urgb_u32(rLine[i]));
        }
#if LED_DEBUG
        printf("Duty[0] %.2f, Duty[%d] %.2f\n", f[0], len - 1, f[len - 1]);
#endif
        sleep_ms(20);
    }
}

void TURN_SIGNAL::pattern_p1_gamma(PATTERN_PARAM_T *pp_p) {
    int len = led_info.num_pixels;
    RGB32_t rLine[len] = {0};
    int max_num = 100;//max brightness
    int p_diff = 2; //phase difference
    float f[len] = {0};
    for (int d = 0; d < max_num + (p_diff * led_info.right_pixels); d += 5 ){
        for (int i =0; i < led_info.right_pixels; i++) {
            int di = 0;
            di = ((d - i*p_diff) < max_num) ? d - i * p_diff : max_num;
            if (di < 0) di = 0;
            f[i] = (float)di / 100.0;
            rLine[i] = setRGB(
                gamma((uint8_t)(f[i] * 0xff), 0.8), 
                gamma((uint8_t)(f[i] * 0x6A), 0.8), 
                0x00);
        }
        for (int i =0; i < len; i++) {
            if (i < led_info.right_pixels) {
                put_pixel((pp_p->right)?
                    urgb_u32(rLine[led_info.right_pixels - 1 -i]):
                    urgb_u32(setRGB(0xff, 0xff, 0xff)));
            } else {
                put_pixel((pp_p->left)?
                    urgb_u32(rLine[i - led_info.right_pixels]):
                    urgb_u32(setRGB(0xff, 0xff, 0xff)));
            }
        }
#if LED_DEBUG
        printf("Duty[0] %.2f, Duty[%d] %.2f\n", f[0], len - 1, f[len - 1]);
#endif
        sleep_ms(23);
    }
    for (int d = max_num; d >= 0 ; d -= 5 ){
        if (d < 0) d=0;
        f[0] = (float)d / 100.0;
#if LED_DEBUG
        printf("Duty[0] %.2f\n", f[0]);
#endif
        rLine[0] = setRGB(
            gamma((uint8_t)(f[0] * 0xff), 0.9), 
            gamma((uint8_t)(f[0] * 0x6A), 0.9), 
            0x00);
        for (int i =0; i < len; i++) {
            if (i < led_info.right_pixels) {
                put_pixel((pp_p->right)?
                    urgb_u32(rLine[0]):
                    urgb_u32(setRGB(0xff, 0xff, 0xff)));
            } else {
                put_pixel((pp_p->left)?
                    urgb_u32(rLine[0]):
                    urgb_u32(setRGB(0xff, 0xff, 0xff)));
            }
        }
        sleep_ms(10);
    }
}

void TURN_SIGNAL::pattern_daylight(pattern_param_t *pp_p) {
    int len = led_info.num_pixels;
    for (int i =0; i < len; i++) {
        put_pixel(urgb_u32(0xff, 0xff, 0xff));
    }
    sleep_ms(200);
}

void TURN_SIGNAL::pattern_daylight_turn_on(pattern_param_t *pp_p) {
    int len = led_info.num_pixels;
    RGB32_t rLine[len] = {0};
    int max_num = 100;//max brightness 
    float f[len] = {0};
#if LED_DEBUG
        printf("Duty[0] %.2f, Duty[%d] %.2f\n", f[0], len - 1, f[len - 1]);
#endif
    for (int d = 0; d < max_num ; d += 5 ){
        if (d > 100) d=0;
        f[0] = (float)d / 100.0;
#if LED_DEBUG
        printf("Duty[0] %.2f\n", f[0]);
#endif
        rLine[0] = setRGB(
            gamma((uint8_t)(f[0] * 0xFF), 0.9), 
            gamma((uint8_t)(f[0] * 0xFF), 0.9), 
            gamma((uint8_t)(f[0] * 0xFF), 0.9));
        for (int i =0; i < len; i++) {
            if (i < led_info.right_pixels) {
                put_pixel((pp_p->right)?
                    urgb_u32(rLine[0]):
                    urgb_u32(setRGB(0xff, 0xff, 0xff)));
            } else {
                put_pixel((pp_p->left)?
                    urgb_u32(rLine[0]):
                    urgb_u32(setRGB(0xff, 0xff, 0xff)));
            }
        }
        sleep_ms(30);
    }
}
