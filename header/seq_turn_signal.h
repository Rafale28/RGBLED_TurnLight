#ifndef __SEQ_TURN_SIGNAL_H__
#define __SEQ_TURN_SIGNAL_H__

#include <math.h>
#include <stdio.h>
#include <string.h>

#include "ws2812.h"
#include "pico/stdlib.h"

#define LED_DEBUG 0
#define SIGNAL_IN_DEBUG 0

#define DEBUG_LED_PIN 25
#define R_SIG_INPUT_PIN 14
#define L_SIG_INPUT_PIN 15

#define INPUT_REG_SIZE 10 

class TURN_SIGNAL : public WS2812 {
public:
    enum LightingPattern {
        SNAKES = 0,
        RANDOM,
        SPARKLES,
        GRAYS,
        RIGHT_WAVE,
        RIGHT_SIN_P1,
        RIGHT_GAMMA_P1,
        RIGHT_SIN_P2,
        LEFT_GAMMA_P1,
        HAZARD,
        DAY,
        DAY_TURN_ON,
        MAX_NUM
    };
private:
    //////////////////////////////////////////////////////////////
    //for LED
    //////////////////////////////////////////////////////////////
    struct led_info_t {
        uint16_t num_pixels;
        uint16_t right_pixels;
        uint16_t left_pixels;
    } led_info;
    RGB32_t setRGB(uint8_t r, uint8_t g, uint8_t b) {
        RGB32_t rgb;
        rgb.r = r;
        rgb.g = g;
        rgb.b = b;
        return rgb;
    }
    float sinCalc(int duty) {
        return sin( duty * M_PI / 180 );
    }
    //typedef struct pattern_param_t {
    //    bool right;
    //    bool left;
    //}
    typedef void (TURN_SIGNAL::*pattern)(uint, uint);
    typedef struct pattern_t{
        pattern pat;
        const char *name;
    } PATTERN_T;
    PATTERN_T pattern_table[MAX_NUM] = {
            {&TURN_SIGNAL::pattern_snakes,          "Snakes!"},
            {&TURN_SIGNAL::pattern_random,          "Random data"},
            {&TURN_SIGNAL::pattern_sparkle,         "Sparkles"},
            {&TURN_SIGNAL::pattern_greys,           "Greys"},
            {&TURN_SIGNAL::pattern_rwave,           "Right Wave"},
            {&TURN_SIGNAL::pattern_right_p1_sin,    "Right sinwave pattern 1"},
            {&TURN_SIGNAL::pattern_right_p1_gamma,  "Right wave w gannma pattern 1"},
            {&TURN_SIGNAL::pattern_right_p2_sin,    "Right sinwave pattern 2"},
            {&TURN_SIGNAL::pattern_left_p1_gamma,   "Left wave w gannma pattern 1"},
            {&TURN_SIGNAL::pattern_hazard,          "Hazard"},
            {&TURN_SIGNAL::pattern_daylight,        "Day"},
            {&TURN_SIGNAL::pattern_daylight_turn_on,"Daylight turn on"}
    };
    uint8_t gamma(uint8_t, float );

    //////////////////////////////////////////////////////////////
    //for Timer
    //////////////////////////////////////////////////////////////
    struct swInputTimer_t{
        int a;
        bool right_en;
        bool left_en;
    };
    swInputTimer_t param;
    struct repeating_timer timer;
    void paramInit() {
        param.a = 114514;
        param.right_en  = false;
        param.left_en   = false;
    }
    static const void regShift(bool *r_in, bool s_in) {
        for (int i=INPUT_REG_SIZE - 1; i >0 ; i--) {
            r_in[i] = r_in[i - 1];
        }
        r_in[0] = s_in;
    }
    static bool regEnCheck(bool *r) {
        bool ret = false;
        for (int i=INPUT_REG_SIZE - 1; i >0 ; i--) {
            ret |= r[i];
        }
        return ret;
    }

    static bool swInputTimerCallback(struct repeating_timer *t){
        static bool led = false;
        static bool r_in[INPUT_REG_SIZE] = {0};
        static bool l_in[INPUT_REG_SIZE] = {0};
        swInputTimer_t *p;

        p = (swInputTimer_t*)(t->user_data);

        gpio_put(DEBUG_LED_PIN, led);
        led = !led;

        bool r_tmp[INPUT_REG_SIZE];
        bool l_tmp[INPUT_REG_SIZE];

        memcpy(r_tmp, r_in, INPUT_REG_SIZE);
        memcpy(l_tmp, l_in, INPUT_REG_SIZE);
        regShift(r_tmp, gpio_get(R_SIG_INPUT_PIN));
        regShift(l_tmp, gpio_get(L_SIG_INPUT_PIN));
        memcpy(r_in, r_tmp, INPUT_REG_SIZE);
        memcpy(l_in, l_tmp, INPUT_REG_SIZE);

        p->right_en = regEnCheck(r_in);
        p->left_en = regEnCheck(l_in);
#if SIGNAL_IN_DEBUG
        printf("Timer!! %lld a->%d SWR:%s SWL:%s\n", time_us_64(), p->a,
            (p->right_en)?"True":"False",
            (p->left_en)?"True":"False");
#endif
        return true;
    }

    //////////////////////////////////////////////////////////////
    //for GPIO
    //////////////////////////////////////////////////////////////
    void gpioInit(){
        gpio_init(DEBUG_LED_PIN);
        gpio_init(R_SIG_INPUT_PIN);
        gpio_init(L_SIG_INPUT_PIN);

        gpio_set_dir(DEBUG_LED_PIN, GPIO_OUT);
        gpio_set_dir(R_SIG_INPUT_PIN, GPIO_IN);
        gpio_set_dir(L_SIG_INPUT_PIN, GPIO_IN);
    }

    int patternChekc(int p_val) {
        if (param.right_en && param.left_en) {
            return HAZARD;
        } else if (param.right_en == true && param.left_en == false) {
            return RIGHT_GAMMA_P1;
        } else if (param.right_en == false && param.left_en == true) {
            return LEFT_GAMMA_P1;
        } else {
            if (p_val != DAY && p_val != DAY_TURN_ON)
                return DAY_TURN_ON;
            else 
                return DAY;
        }
    }
public:
    void pattern_snakes(uint len, uint t);
    void pattern_random(uint len, uint t);
    void pattern_sparkle(uint len, uint t);
    void pattern_greys(uint len, uint t);
    void pattern_rwave(uint len, uint t);

    void pattern_right_p1_sin(uint len, uint t);
    void pattern_right_p1_gamma(uint len, uint t);
    void pattern_right_p2_sin(uint len, uint t);

    void pattern_left_p1_gamma(uint len, uint t);

    void pattern_hazard(uint len, uint t);
    void pattern_daylight(uint len, uint t);
    void pattern_daylight_turn_on(uint len, uint t);

    void startTimer() {
        paramInit();
        add_repeating_timer_ms(50, swInputTimerCallback, (void *)&param, &timer);
    }
    void run(int t) {
        int pattern = MAX_NUM;
        while(true) {
            printf("ENABLE CHECK!! SWR:%s SWL:%s\n",
                param.right_en?"True":"False",
                param.left_en?"True":"False");
            pattern = patternChekc(pattern);
            puts(getName(pattern));
            (this->*pattern_table[pattern].pat)(led_info.num_pixels, t);
        }
    }
    int getPcount() {
        return count_of(pattern_table);
    }
    const char *getName(int ptn) {
        return this->pattern_table[ptn].name;
    }
    uint16_t getPixels() {
        return led_info.num_pixels;
    }

    TURN_SIGNAL(uint16_t np = 50) {
        led_info.num_pixels = np;
        led_info.right_pixels = np >> 1;
        led_info.left_pixels = np >> 1;
        gpioInit();
    }
};

#endif
