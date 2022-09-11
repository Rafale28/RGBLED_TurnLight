#ifndef __SEQ_TURN_SIGNAL_H__
#define __SEQ_TURN_SIGNAL_H__

#include <math.h>
#include <stdio.h>
#include <string.h>

#include "version.h"
#include "ws2812.h"
#include "pico/stdlib.h"

#define LED_DEBUG 0
#define SIGNAL_IN_DEBUG 0

#define DEBUG_LED_PIN 25
#define R_SIG_INPUT_PIN 14
#define L_SIG_INPUT_PIN 15

#define INPUT_REG_SIZE 10 

class TURN_SIGNAL : public WS2812 {
private:
    enum LightingPattern {
        SNAKES = 0,
        RANDOM,
        SPARKLES,
        GRAYS,
        //WAVE,
        SIN_P1,
        GAMMA_P1,
        SIN_P2,
        DAY,
        DAY_TURN_ON,
        PATTERN_MAX_NUM
    };
    enum Mode {
        DEFAULT = 0,
        DEBUG,
        MODE_MAX_NUM
    };
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
    typedef struct pattern_param_t {
        int pattern;
        bool right;
        bool left;
    }PATTERN_PARAM_T;
    typedef void (TURN_SIGNAL::*pattern)(PATTERN_PARAM_T *);
    typedef struct pattern_t{
        pattern pat;
        const char *name;
    } PATTERN_T;
    PATTERN_T pattern_table[PATTERN_MAX_NUM] = {
            {&TURN_SIGNAL::pattern_snakes,          "Snakes!"},
            {&TURN_SIGNAL::pattern_random,          "Random data"},
            {&TURN_SIGNAL::pattern_sparkle,         "Sparkles"},
            {&TURN_SIGNAL::pattern_greys,           "Greys"},
         //   {&TURN_SIGNAL::pattern_rwave,           "Right Wave"},
            {&TURN_SIGNAL::pattern_p1_sin,          "sinwave pattern 1"},
            {&TURN_SIGNAL::pattern_p1_gamma,        "wave w gannma pattern 1"},
            {&TURN_SIGNAL::pattern_p2_sin,          "sinwave pattern 2"},
            {&TURN_SIGNAL::pattern_daylight,        "Day"},
            {&TURN_SIGNAL::pattern_daylight_turn_on,"Daylight turn on"}
    };
    void setPatternParam(PATTERN_PARAM_T *, int, bool, bool);

    //////////////////////////////////////////////////////////////
    //for Timer
    //////////////////////////////////////////////////////////////
    typedef struct swInputTimer_t{
        bool right_en;
        bool left_en;
    }SW_INPUT_STATE_T;
    struct repeating_timer timer;
    void initSwInputParam(SW_INPUT_STATE_T *);
    static const void regShift(bool *, bool);
    static bool regEnCheck(bool *); 

    static bool swInputTimerCallback(struct repeating_timer *);

    //////////////////////////////////////////////////////////////
    //for GPIO
    //////////////////////////////////////////////////////////////
    void initGpio();
    int chekcPattern(PATTERN_PARAM_T *, SW_INPUT_STATE_T *);
    void startTimer(SW_INPUT_STATE_T *);

    //////////////////////////////////////////////////////////////
    //pattern
    //////////////////////////////////////////////////////////////
    void pattern_snakes(PATTERN_PARAM_T *);
    void pattern_random(PATTERN_PARAM_T *);
    void pattern_sparkle(PATTERN_PARAM_T *);
    void pattern_greys(PATTERN_PARAM_T *);
    void pattern_rwave(PATTERN_PARAM_T *);

    void pattern_p1_sin(PATTERN_PARAM_T *);
    void pattern_p1_gamma(PATTERN_PARAM_T *);
    void pattern_p2_sin(PATTERN_PARAM_T *);

    void pattern_daylight(PATTERN_PARAM_T *);
    void pattern_daylight_turn_on(PATTERN_PARAM_T *);

    void showPatternParam(PATTERN_PARAM_T *pp_p);

    uint8_t gamma(uint8_t, float );
    float sinCalc(int);
    
    int getPcount();
    const char *getName(int ptn);
    uint16_t getPixels();
public:
    void run();

    TURN_SIGNAL(uint16_t np = 50) {
        led_info.num_pixels = np;
        led_info.right_pixels = np >> 1;
        led_info.left_pixels = np >> 1;
        initGpio();
    }
};

#endif
